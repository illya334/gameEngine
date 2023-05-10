#ifndef __engineL0_h__
	#define __engineL0_h__
	
	#include <Windows.h>
	#include <gl/GL.h>
	#include <gl/GLU.h>
	#include <cstdint>

	#include "fuse.h"
	#include "define.h"

	#pragma comment(lib, "opengl32.lib")
	#pragma comment(lib, "glu32.lib")

	extern UINT windowWidth;
	extern UINT windowHeight;
	extern bool fullscreen;
	extern bool activeWindow;
	extern char keys[256];
	extern float FOV;
	extern float MDR;
	extern bool modeOrtho;
	extern float mouseX;
	extern float mouseY;
	extern short mouseDifX;
	extern short mouseDifY;

	struct stStatusSystem{
		const char* OSname;
		const char* CPUarchitecture;
		const bool supportKeyboard;
		const bool supportMouse;
	};
	extern const struct stStatusSystem statusSystem;

	void engineL0Init(void (*funcStart)(), void (*funcLoop)(), void (*funcFrame)(), void (*funcEnd)());
	void errorInGame(const char* str, int errCode);
	void shutdownEngine();

	bool addButtonHandling(word index, bool hold);
	bool subButtonHandling(word index);
	byte getButtonState(word index);
	bool getButtonPress(word index);
	bool getButtonHold(word index);
	bool getButtonReleased(word index);

	UINT getTickTime();

	void setFOV(float newFOV);
	void setMDR(float newMDR);
	void setFullScreen(bool mode);
	void setOrtho(bool mode);

	uint openFile(char* fileName, bool typeRead, bool typeWrite, bool typeCreate);
	uint getFileSize(uint file);
	uint readFile(uint file, void* buf, uint bytesToRead /*if bytesToRead = -1 then will read all file*/, uint offset);
	uint writeFile(uint file, void* buf, uint bytesToWrite, uint offset);
	bool closeFile(uint file);
	bool deleteFile(char* fileName);

#endif