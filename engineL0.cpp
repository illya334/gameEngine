/*
	by Parfonov Illya (zeq52giw | illya334)
	Time: 27.04.2023 15:26

	TODO: fullscreen
*/

#include "engineL0.h"

// PUBLIC
UINT windowWidth = FUSE_STD_WINDOW_WIDTH;
UINT windowHeight = FUSE_STD_WINDOW_HEIGHT;
bool fullscreen = FUSE_STD_FULLSCREEN;
bool activeWindow = false;
char keys[256] = {};
float FOV = FUSE_STD_FOV;
float MDR = FUSE_STD_MDR;
bool modeOrtho = FUSE_MODE_ORTHO;


// PRIVAT
HDC hDC;
HGLRC hRC;
HWND ghWnd;
word* _keysIndexHandle = 0; // Stack indexs
word* _keysButtonHandle = 0; // Result handling
bool _handlingKeysEnable = false;
RECT rect;
WINDOWPLACEMENT wpc;
bool wasChangedFullscreen = true;
bool shutdownEng = false;
void (*cfuncEnd)();


void setFullScreen(bool mode);

LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);

void errorFunc(const char *str) {
	int err = GetLastError();
	char* buf = (char*)LocalAlloc(LPTR, strlen(str) + 30);
	wsprintfA(buf, "ERROR: In func '%s'. Code: 0x%X", str, err);
	MessageBoxA(NULL, buf, "ERROR Engine", MB_OK | MB_ICONERROR);
	ExitProcess(err);
}

void errorInGame(const char* str, int errCode) {
	char* buf = (char*)LocalAlloc(LPTR, strlen(str) + 30);
	wsprintfA(buf, "ERROR: %s. Code: 0x%X", str, errCode);
	MessageBoxA(NULL, buf, "ERROR Engine", MB_OK | MB_ICONERROR);
	ExitProcess(errCode);
}

void shutdownEngine() {
	shutdownEng = true;
}

void _handlingKeysFunc() {
	/* _keysButtonHandle: (bits)
		0 - pressed
		1 - hold
		2 - Released
		3 - was cleaned PRESSed, set 0 if was Released
		4 - was cleaned HOLD, set 0 if was Released
		5 - was cleaned Released

		6 - 15 bit is Tick for hold
		(GetTickCount() / 10) & 0x3FF) = 1304350 / 10 = 130435 & 0x3FF = 183;
	*/


	for (uint i = 0; i < 256; i++) {
		if (_keysIndexHandle[i] == 0) break;

		bool hold = (_keysIndexHandle[i] & 0x8000) >> 15;
		word index = _keysIndexHandle[i] & ~0x8000; // clear hold bit
		
		if (keys[index]) {
			if (_keysButtonHandle[index] & 1) { // for hold
				if (hold && !(_keysButtonHandle[index] & (1 << 1)))
					if (((GetTickCount() / 10) & 0x3FF) - ((_keysButtonHandle[index] & 0xFFC0) >> 6) > FUSE_KEYHOLD_TIME/10)
						_keysButtonHandle[index] |= (1 << 1); // set hold bit

			} else { // set PRESSed
				_keysButtonHandle[index] |= 1; // set bit Press
				_keysButtonHandle[index] &= ~((1 << 2) | (1 << 5)); // clear bit Released, wasReleased

				if (hold) 
					_keysButtonHandle[index] = (((GetTickCount()/10) & 0x3FF) << 6) | (_keysButtonHandle[index] & 0x3F);
			}
		}
		else {
			if (!(_keysButtonHandle[index] & (1 << 2)) && (_keysButtonHandle[index] != 0)) { // Released
				_keysButtonHandle[index] |= (1 << 2); // set bit Released
				_keysButtonHandle[index] &= ~(1 | (1 << 1) | (1 << 3) | (1 << 4)); // clear bit Press, hold, wasPress, wasHold
			}
		}
	}
}

void openGLInit(HWND hwnd) {
	static	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR), 1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA,
		16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 0, 0, PFD_MAIN_PLANE, 0, 0, 0, 0
	};

	hDC = GetDC(hwnd);
	GetClientRect(hwnd, &rect);

	register int PixelFormat = ChoosePixelFormat(hDC, &pfd);
	if (!PixelFormat) errorFunc("ChoosePixelFormat");

	if (!SetPixelFormat(hDC, PixelFormat, &pfd)) errorFunc("SetPixelFormat");

	hRC = wglCreateContext(hDC);
	if (!hRC) errorFunc("wglCreateContext");

	if (!wglMakeCurrent(hDC, hRC)) errorFunc("wglMakeCurrent");

	glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float tmpProporc = (float)rect.right / (float)rect.bottom;

	if (!modeOrtho) gluPerspective(FOV, tmpProporc, 0.1f, MDR);
	else glOrtho(-1.0f * tmpProporc, 1.0f * tmpProporc, -1.0f, 1.0f, 0.1f, MDR);

	glMatrixMode(GL_MODELVIEW);
}

void reSizeGLScene(int width, int height){
	if (height == 0) height = 1;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float tmpProporc = (float)width / (float)height;

	if (!modeOrtho) gluPerspective(FOV, tmpProporc, 0.1f, MDR);
	else glOrtho(-1.0f * tmpProporc, 1.0f * tmpProporc, -1.0f, 1.0f, 0.1f, MDR);

	glMatrixMode(GL_MODELVIEW);
}

void OpenGLoff() {
	//ChangeDisplaySettingsA(NULL, 0);

	wglMakeCurrent(hDC, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(ghWnd, hDC);
}

// PUBLIC
void engineL0Init( void (*funcStart)(), void (*funcLoop)(), void (*funcFrame)(), void (*funcEnd)()) { // забирає все управління на себе
	_keysIndexHandle = (word*)LocalAlloc(LPTR, sizeof(word) * 257);
	_keysButtonHandle = (word*)LocalAlloc(LPTR, sizeof(word) * 256);
	
	cfuncEnd = funcEnd;

	HINSTANCE hInstance = GetModuleHandleA(NULL);
	MSG msg;
	WNDCLASSA wnd = {NULL, &wndProc, NULL, NULL, hInstance, NULL, NULL, NULL, NULL, "Engine"};

	register ATOM tmpATOM = RegisterClassA(&wnd);
	if (tmpATOM == NULL)	errorFunc("RegisterClassA");

	HWND ghWnd = CreateWindowExA(0, (LPCSTR)tmpATOM, "gameName", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeight, 0, 0, hInstance, 0);
	if (ghWnd == NULL) errorFunc("CreateWindowExA");

	funcStart();

	#ifdef FUSE_FULLSCREEN_F11
		addButtonHandling(VK_F11, false);
	#endif

	while (true) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}

		if (shutdownEng) {
			OpenGLoff();
			funcEnd();
			ExitProcess(0);
		}

		funcLoop();

		if (activeWindow) {
			funcFrame();
			SwapBuffers(hDC);
		}

		_handlingKeysFunc();

		#ifdef FUSE_FULLSCREEN_F11
			if (getButtonReleased(VK_F11)) setFullScreen(!fullscreen);
		#endif

		if(wasChangedFullscreen){
			if (fullscreen) {
				int screenWidth = GetSystemMetrics(SM_CXSCREEN);
				int screenHeight = GetSystemMetrics(SM_CYSCREEN);

				SetWindowLongA(ghWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
				SetWindowPos(ghWnd, HWND_TOPMOST, 0, 0, screenWidth, screenHeight, SWP_FRAMECHANGED);
			} else {
				SetWindowLongA(ghWnd, GWL_STYLE, WS_VISIBLE | WS_OVERLAPPEDWINDOW);
				SetWindowPos(ghWnd, HWND_TOP, 0, 0, FUSE_STD_WINDOW_WIDTH, FUSE_STD_WINDOW_HEIGHT, SWP_FRAMECHANGED);
			}

			wasChangedFullscreen = false;
		}

		#ifdef FUSE_DELAY_TICK
			Sleep(FUSE_DELAY_TICK);
		#endif // FUSE_DELAY
	}

}

LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	
	switch (msg) {

		case WM_CREATE:
			openGLInit(hwnd);
		break;

		case WM_DESTROY:
			cfuncEnd();
			OpenGLoff();
			ExitProcess(NULL);
		break;

		case WM_ACTIVATE:
			activeWindow = (wParam > 0) ? true : false;
		break;

		case WM_KEYDOWN:
			keys[wParam] = true;
		break;

		case WM_KEYUP:
			keys[wParam] = false;
		break;

		case WM_SIZE:
			windowWidth = LOWORD(lParam);
			windowHeight = HIWORD(lParam);
			reSizeGLScene(LOWORD(lParam), HIWORD(lParam));
		break;

		default:
			return DefWindowProcA(hwnd, msg, wParam, lParam);
	}

	return 0;
}

/* Example:
	addButtonHandling('A', true); // Додаємо кнопку у список обробки
	if( getButtonPress('A') ) printf("PRESSed!"); // Виводить на екран тільки при натискані кнопки, виводить на екран тільки один раз.
	if( getButtonHold('A') ) printf("HOLD!"); // Спрацьовує при затриманні кнопки у натиснутому стані. Час затримки можно налаштувати у fuses.h > fuseKeysHoldTime.
	if( getButtonReleased('A') ) printf("RELEASed!"); // Спрацьовує при відпусканні кнопки. Спрацьовує тільки один раз.
	subButtonHandling('A'); // Вимикаємо обробку цієї кнопки.
*/

// Увага!!! Немає перевірки на правельність INDEX-у.
// Index брати з keys[], тобто у 'engineL0.h' є список макросів які може приймати масив та функції. Якщо вам потрібно використати звичайні кнопки (A-Z, 0-9), то використовуємо такий запис: 'A', 'B', '0', '1';

bool addButtonHandling(word index, bool hold) { // Додає кнопку до список обробки. Index - тіж самі, які є у keys[]. Hold - обробляти затримку кнопки?
	if (!FUSE_SUPPORT_KEYBOARD) return false;

	_handlingKeysEnable = true;

	for (uint i = 0; i < 256; i++) { // search free space
		if (_keysIndexHandle[i] == index) {
			if (hold)
				_keysIndexHandle[i] |= 0x8000; // 16 bit - hold
			break;
		}
		if (_keysIndexHandle[i] == 0) {
			_keysIndexHandle[i] = index;
			if (hold)
				_keysIndexHandle[i] |= 0x8000; // 16 bit - hold
			_keysIndexHandle[i + 1] = 0;
			break;
		}
	}

	return true;
}

bool subButtonHandling(word index) { // Видоляє кнопку зі списку обробки. Index - брати той який був у addButtonHandling
	if (!FUSE_SUPPORT_KEYBOARD) return false;

	for (uint i = 0; i < 256; i++) {
		if (_keysIndexHandle[i] == index) {
			for (uint j = i; j < 256; j++) {
				if (j + 1 < 256)
					_keysIndexHandle[j] = _keysIndexHandle[j + 1];

				if (_keysIndexHandle[j] == 0) break;
			}
			break;
		}
	}

	return true;
}


/* Return byte:
	0 - pressed
	1 - hold
	2 - Released
	3 - was cleaned PRESSed
	4 - was cleaned HOLD
	5 - was cleaned Released
*/
byte getButtonState(word index) { // Повертає стан копки. Не змінює флаги.
	if (!FUSE_SUPPORT_KEYBOARD) return false;
	return (byte)(_keysButtonHandle[index] & 0x3F);
}

bool getButtonPress(word index) { // Повертає стан кнопки, якщо кнопка натиснута. Змінює флаги, тобто якщо використати цю функцію повторно то поверне false, та поверне true коли натисниться кнопка знову.
	if (!FUSE_SUPPORT_KEYBOARD) return false;

	if ((_keysButtonHandle[index] & 1) && !(_keysButtonHandle[index] & (1 << 3))) {
		_keysButtonHandle[index] |= (1 << 3);
		return true;
	}
	return false;
}

bool getButtonHold(word index) { // Повертає стан кнопки, якщо кнопка натиснута та затримується. Змінює флаги. ^^^
	if (!FUSE_SUPPORT_KEYBOARD) return false;

	if ((_keysButtonHandle[index] & (1 << 1)) && !(_keysButtonHandle[index] & (1 << 4))) {
		_keysButtonHandle[index] |= (1 << 4);
		return true;
	}
	return false;
}

bool getButtonReleased(word index) { // Повертає стан кнопки, якщо кнопка була відтиснута. Змінює флаги.       ^^^
	if (!FUSE_SUPPORT_KEYBOARD) return false;

	if ((_keysButtonHandle[index] & (1 << 2)) && !(_keysButtonHandle[index] & (1 << 5))) {
		_keysButtonHandle[index] |= (1 << 5);
		return true;
	}
	return false;
}

unsigned int getTickTime() {
	return GetTickCount();
}

void setFOV(float newFOV) {
	FOV = newFOV;

	GetClientRect(ghWnd, &rect);
	reSizeGLScene(rect.right, rect.bottom);
}

void setMDR(float newMDR) {
	MDR = newMDR;

	GetClientRect(ghWnd, &rect);
	reSizeGLScene(rect.right, rect.bottom);
}

void setFullScreen(bool mode) {
	fullscreen = mode;
	wasChangedFullscreen = true;
}

void setOrtho(bool mode) {
	modeOrtho = mode;
	reSizeGLScene(windowWidth, windowHeight);
}