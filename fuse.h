// This file for settings.

#ifndef __fuse_h__
#define __fuse_h__

	#define FUSE_STD_FOV 60

	#define FUSE_STD_MDR 100

	//#define FUSE_MODE_ORTHO // Comment out the line to disable.

	//#define FUSE_STD_FULLSCREEN // Comment out the line to disable.

	#define FUSE_STD_WINDOW_WIDTH 500
	
	#define FUSE_STD_WINDOW_HEIGHT 500

	#define FUSE_DELAY_TICK 10 // delay in N milliseconds each tick. Comment out the line to disable.

	#define FUSE_KEYHOLD_TIME 300 // ms

	#define FUSE_SUPPORT_KEYBOARD // Comment out the line to disable.
 	
	#define FUSE_FULLSCREEN_F11 // Comment out the line to disable.

// =======================================================================================

	#ifdef FUSE_SUPPORT_KEYBOARD
		#define FUSE_SUPPORT_KEYBOARD true
	#else
		#define FUSE_SUPPORT_KEYBOARD false
	#endif

	#ifdef FUSE_MODE_ORTHO
		#define FUSE_MODE_ORTHO true
	#else
		#define FUSE_MODE_ORTHO false
	#endif

	#ifdef FUSE_STD_FULLSCREEN
		#define FUSE_STD_FULLSCREEN true
	#else
		#define FUSE_STD_FULLSCREEN false
	#endif

#endif