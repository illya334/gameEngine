#include "engineL1.h"
#include "objectSystem.h"

#include <cstdio>

void start() {
	addButtonHandling('A', false);
	addButtonHandling('S', false);
	addButtonHandling(VK_ESCAPE, false);

	objectSystemInit();

	struct stGroupObject *mainGroup = getMainGroup();

	createObject(mainGroup, obj_point, null, null, null);

	struct stGroupObject* group = (struct stGroupObject*)createObject((struct stGroupObject*)obj_nogoup, obj_group, null, null, null);

	createObject(group, obj_point, null, null, null);

	moveObject(group, mainGroup);

	debugObject(mainGroup);
}

void loop() {
	if (getButtonReleased('A')) glLoadIdentity();
	if (getButtonReleased('S')) setOrtho(!modeOrtho);
	if (getButtonReleased(VK_ESCAPE)) shutdownEngine();
}

void frame() {
	//glLoadIdentity();

	//glRotatef(A, 0, 1, 0);
	glTranslatef(0, 0, -0.5);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glBegin(GL_POINTS);
		
	glEnd();
}

void end() {
	printf("NOOOooo\n");
}

void engineL1Init() {
	engineL0Init(&start, &loop, &frame, &end);
}

void drawPoint(float X, float Y, float Z, byte red, byte green, byte blue) { // TODO

}