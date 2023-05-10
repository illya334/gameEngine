#ifndef __engineL1_h__
	#define __engineL1_h__

	#include "engineL0.h"

	void engineL1Init();
	
	struct stPointObject* drawPoint(float X, float Y, float Z, byte red, byte green, byte blue);
	struct stRectangleObject* drawRectangle(float X1, float Y1, float Z1, float X2, float Y2, float Z2, float X3, float Y3, float Z3, float X4, float Y4, float Z4, byte red, byte green, byte blue);
	struct stRectangleObject* drawRectangleGeneralZ(float X1, float Y1, float X2, float Y2, float X3, float Y3, float X4, float Y4, float Z, byte red, byte green, byte blue);

	// return textureObj
	uint loadTexture(char* filename, int levelDetal /*0 - 3*/);

#endif