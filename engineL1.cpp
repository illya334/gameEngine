/*
	by Illya Parfonov

	TODO: Отображение обьектов - создать список и записывать в него обьекты
*/

#include "engineL1.h"
#include "objectSystem.h"

#include <cstdio>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "stb_image.h"

struct stPointObject* point;

//GLuint (*glCreateProgram)();

stRectangleObject* rec;

void start() {
	objectSystemInit();
	glEnable(GL_TEXTURE_2D);

	addButtonHandling('R', false);
	addButtonHandling('T', false);
	addButtonHandling(VK_ESCAPE, false);

	point = drawPoint(0, 0, -1, 255, 0, 0);

	drawRectangleGeneralZ(
					-0.5+10, 0.5, // 1
					0.5+10, 0.5, // 2
					-0.5+10, -0.5, // 3
					0.5+10, -0.5, // 4
					-3, // Z
					0, 255, 125); // color

	char* verOpenGL = (char*)glGetString(GL_VERSION);
	printf("VER: %s\n", verOpenGL);

	//glCreateProgram = (GLuint (*)())wglGetProcAddress("glCreateProgram");

	struct stCoord pos = { -0.5, 0.5, -3 };
	st2DSpriteObject *sprite = (st2DSpriteObject*)createObject(mainGroup, obj_2DSprite | obj_bVisible, &pos, null, null);
	sprite->textureObj = loadTexture((char*)"1.jpg", 0);
	if (sprite->textureObj == 0) printf("ERROR load file\n");
	sprite->pos2.x = -0.5;
	sprite->pos2.y = -0.5;
	sprite->pos2.z = -3;
	sprite->color.red = 255;

	struct st2DSpriteObject* obj = (struct st2DSpriteObject*)findObjectByType(mainGroup, obj_2DSprite, 0);
	printf("%X\n", obj);

	rec = drawRectangleGeneralZ(
		-0.5, 0.5, // 1
		0.5, 0.5, // 2
		-0.5, -0.5, // 3
		0.5, -0.5, // 4
		3, // Z
		0, 255, 0); // color

	mainCamera->setPosZ(-2);
	//mainCamera->setAngleY(180);
}

const float stepSize = 0.05;
const float mouseCoef = 0.005;

void loop() {
	if (getButtonReleased('R')) {
		mainCamera->pos.x = 0;
		mainCamera->pos.y = 0;
		mainCamera->pos.z = 0;
	}
	if (getButtonReleased('T')) setOrtho(!modeOrtho);
	if (getButtonReleased(VK_ESCAPE)) shutdownEngine();

	if (keys['W']) {
		mainCamera->pos.x += cosf((mainCamera->angle.y + 90) * 3.141592 / 180) * stepSize;
		mainCamera->pos.y += sinf((mainCamera->angle.x) * 3.141592 / 180) * stepSize;
		mainCamera->pos.z -= sinf((mainCamera->angle.y + 90) * 3.141592 / 180) * stepSize;
	}
	if (keys['S']) {
		mainCamera->pos.x -= cosf((mainCamera->angle.y + 90) * 3.141592 / 180) * stepSize;
		mainCamera->pos.y -= sinf((mainCamera->angle.x) * 3.141592 / 180) * stepSize;
		mainCamera->pos.z += sinf((mainCamera->angle.y + 90) * 3.141592 / 180) * stepSize;
	}
	if (keys['A']) {
		mainCamera->pos.x -= cosf((mainCamera->angle.y) * 3.141592 / 180) * stepSize;
		//mainCamera->pos.y += sinf((mainCamera->angle.x-90) * 3.141592 / 180) * stepSize;
		mainCamera->pos.z += sinf((mainCamera->angle.y) * 3.141592 / 180) * stepSize;
	}
	if (keys['D']) {
		mainCamera->pos.x += cosf((mainCamera->angle.y) * 3.141592 / 180) * stepSize;
		//mainCamera->pos.y -= sinf((mainCamera->angle.x-90) * 3.141592 / 180) * stepSize;
		mainCamera->pos.z -= sinf((mainCamera->angle.y) * 3.141592 / 180) * stepSize;
	}
	if (keys[VK_LEFT]) mainCamera->angle.y += 1;
	if (keys[VK_RIGHT]) mainCamera->angle.y -= 1;
	if (keys[VK_UP]) mainCamera->angle.x += 1;
	if (keys[VK_DOWN]) mainCamera->angle.x -= 1;

	//if (keys['U']) rec->angle.y += 1;

	//mainCamera->angle.y += mouseDifY * mouseCoef;
	//mainCamera->angle.x += mouseDifX * mouseCoef;

	//printf("X: %d, Y: %d; BX: %d, BY: %d\n", mouseDifX, mouseDifY, mouseX, mouseY);

	point->pos.x += 0.001;
}

void frame() {
	glLoadIdentity();

	glRotatef(-(mainCamera->angle.x), 1, 0, 0);
	glRotatef(-(mainCamera->angle.y), 0, 1, 0);
	glRotatef(-(mainCamera->angle.z), 0, 0, 1);
	glTranslatef(-(mainCamera->pos.x), -(mainCamera->pos.y), -(mainCamera->pos.z));

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);

	glBegin(GL_POINTS);
	for (uint i = 0; ; i++) {
		struct stEmtyObject* obj = findObjectByType(mainGroup, obj_point, i);
		if (obj == null) break;
		if ((obj->type & obj_bVisible) != obj_bVisible) continue;
		glColor4ub(obj->color.red, obj->color.green, obj->color.blue, obj->color.alpha);
		glVertex3f(obj->pos.x, obj->pos.y, obj->pos.z);
	}
	glEnd();

	glBegin(GL_QUADS);
		for (uint i = 0; ; i++) {
			struct stRectangleObject* obj = (struct stRectangleObject*)findObjectByType(mainGroup, obj_rectangle, i);
			if (obj == null) break;
			if ((obj->type & obj_bVisible) != obj_bVisible) continue;

			glColor4ub(obj->color.red, obj->color.green, obj->color.blue, obj->color.alpha);
			glVertex3f(obj->pos.x, obj->pos.y, obj->pos.z);
			glVertex3f(obj->pos3.x, obj->pos3.y, obj->pos3.z);
			glVertex3f(obj->pos4.x, obj->pos4.y, obj->pos4.z);
			glVertex3f(obj->pos2.x, obj->pos2.y, obj->pos2.z);
		}

		for (uint i = 0; ; i++) {
			struct st2DSpriteObject* obj = (struct st2DSpriteObject*)findObjectByType(mainGroup, obj_2DSprite, i);
			if (obj == null) break;
			if ((obj->type & obj_bVisible) != obj_bVisible) continue;

			glColor4ub(255, obj->color.green, obj->color.blue, 255);
			glVertex3f(obj->pos.x, obj->pos.y, obj->pos.z);
			glVertex3f(obj->pos2.x, obj->pos.y, obj->pos.z);
			glVertex3f(obj->pos2.x, obj->pos2.y, obj->pos.z);
			glVertex3f(obj->pos.x, obj->pos2.y, obj->pos.z);
		}
	glEnd();

	// НЕ РАБОТАЕТ!!!!
	/* glBegin(GL_QUADS);
	for (uint i = 0; ; i++) {
		struct st2DSpriteObject* obj = (struct st2DSpriteObject*)findObjectByType(mainGroup, obj_2DSprite, i);
		if (obj == null) break;
		if ((obj->type & obj_bVisible) != obj_bVisible) continue;

		glColor4ub(255, obj->color.green, obj->color.blue, 255);
		glVertex3f(obj->pos.x, obj->pos.y, obj->pos.z);
		glVertex3f(obj->pos2.x, obj->pos.y, obj->pos.z);
		glVertex3f(obj->pos2.x, obj->pos2.y, obj->pos.z);
		glVertex3f(obj->pos.x, obj->pos2.y, obj->pos.z);
	}
	glEnd();*/

}

void end() {
	glDisable(GL_TEXTURE_2D);
	printf("NOOOooo\n");
}

void engineL1Init() {
	// OpenGL not initialized!
	engineL0Init(&start, &loop, &frame, &end);
}

struct stPointObject* drawPoint(float X, float Y, float Z, byte red, byte green, byte blue) { // TODO
	struct stCoord _coord;
	_coord.x = X;
	_coord.y = Y;
	_coord.z = Z;
	struct stPointObject* _point = (struct stPointObject*)createObject(baseGeometry, obj_point | obj_bVisible, &_coord, null, null);
	_point->color.red = red;
	_point->color.green = green;
	_point->color.blue = blue;
	return _point;
}

struct stRectangleObject* drawRectangle(
		float X1, float Y1, float Z1,
		float X2, float Y2, float Z2,
		float X3, float Y3, float Z3,
		float X4, float Y4, float Z4,
		byte red, byte green, byte blue
	) {
	struct stCoord _coord = {X1, Y1, Z1};
	struct stRectangleObject* _rectangle = (struct stRectangleObject*)createObject(baseGeometry, obj_rectangle | obj_bVisible, &_coord, null, null);
	_rectangle->color.red = red;
	_rectangle->color.green = green;
	_rectangle->color.blue = blue;
	_rectangle->pos2.x = X2;
	_rectangle->pos2.y = Y2;
	_rectangle->pos2.z = Z2;
	_rectangle->pos3.x = X3;
	_rectangle->pos3.y = Y3;
	_rectangle->pos3.z = Z3;
	_rectangle->pos4.x = X4;
	_rectangle->pos4.y = Y4;
	_rectangle->pos4.z = Z4;

	return _rectangle;
}

struct stRectangleObject* drawRectangleGeneralZ(
	float X1, float Y1,
	float X2, float Y2,
	float X3, float Y3,
	float X4, float Y4,
	float Z,
	byte red, byte green, byte blue) {
	return drawRectangle(
		X1, Y1, Z,
		X2, Y2, Z,
		X3, Y3, Z,
		X4, Y4, Z,
		red, green, blue);
}

uint loadTexture(char* fileName, int levelDetal) {
	int w, h, comp;
	uint textureObj;
	char *data = (char*)stbi_load(fileName, &w, &h, &comp, 0);
	if (data == 0) return 0;

	glGenTextures(1, &textureObj);
	glBindTexture(GL_TEXTURE_2D, textureObj);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, levelDetal, GL_RGBA, w, h, 0, comp == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
	return textureObj;
}

