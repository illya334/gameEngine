#ifndef __DEFINE__
	#define __DEFINE__

	#include <cstdint>

	#define byte uint8_t
	#define word uint16_t
	#define dword uint32_t
	#define qword uint64_t

	#define uint unsigned int

	#define null NULL

	// Virtual keys...
	// mouse
	#define VK_LBUTTON              0x01 // Left mouse button
	#define VK_RBUTTON              0x02 // Right mouse button
	#define VK_CANCEL               0x03 // Control-break processing
	#define VK_MBUTTON              0x04 // Middle mouse button (three-button mouse)
	#define VK_XBUTTON1             0x05 // X1 mouse button
	#define VK_XBUTTON2             0x06 // X2 mouse button

	#define VK_BACK                 0x08 // BACKSPACE key
	#define VK_TAB                  0x09 // TAB key
	#define VK_CLEAR                0x0C // CLEAR key
	#define VK_RETURN               0x0D // ENTER key
	#define VK_SHIFT                0x10 // SHIFT key
	#define VK_CONTROL              0x11 // CTRL key
	#define VK_MENU                 0x12 // ALT key
	#define VK_PAUSE                0x13 // PAUSE key
	#define VK_CAPITAL              0x14 // CAPS LOCK key
	#define VK_ESCAPE               0x1B // ESC key
	#define VK_SPACE                0x20 // SPACEBAR
	#define VK_PRIOR                0x21 // PAGE UP key
	#define VK_NEXT                 0x22 // PAGE DOWN key
	#define VK_END                  0x23 // END key
	#define VK_HOME                 0x24 // HOME key
	#define VK_LEFT                 0x25 // LEFT ARROW key
	#define VK_UP                   0x26 // UP ARROW key
	#define VK_RIGHT                0x27 // RIGHT ARROW key
	#define VK_DOWN                 0x28 // DOWN ARROW key
	#define VK_INSERT               0x2D // INS key
	#define VK_DELETE               0x2E // DEL key

	#define VK_LWIN                 0x5B // Left Windows key (Natural keyboard)
	#define VK_RWIN                 0x5C // Right Windows key (Natural keyboard)

	// NUMPAD
	#define VK_NUMPAD0              0x60 // Numeric keypad 0 key
	#define VK_NUMPAD1              0x61 // Numeric keypad 1 key
	#define VK_NUMPAD2              0x62 // Numeric keypad 2 key
	#define VK_NUMPAD3              0x63 // Numeric keypad 3 key
	#define VK_NUMPAD4              0x64 // Numeric keypad 4 key
	#define VK_NUMPAD5              0x65 // Numeric keypad 5 key
	#define VK_NUMPAD6              0x66 // Numeric keypad 6 key
	#define VK_NUMPAD7              0x67 // Numeric keypad 7 key
	#define VK_NUMPAD8              0x68 // Numeric keypad 8 key
	#define VK_NUMPAD9              0x69 // Numeric keypad 9 key
	#define VK_MULTIPLY             0x6A // Multiply key (*)
	#define VK_ADD                  0x6B // Add key (+)
	#define VK_SEPARATOR            0x6C // Separator key ??? i don't know want is it
	#define VK_SUBTRACT             0x6D // Subtract key (-)
	#define VK_DECIMAL              0x6E // Decimal key (.)
	#define VK_DIVIDE               0x6F // Divide key (/)

	// Fn
	#define VK_F1                   0x70
	#define VK_F2                   0x71
	#define VK_F3                   0x72
	#define VK_F4                   0x73
	#define VK_F5                   0x74
	#define VK_F6                   0x75
	#define VK_F7                   0x76
	#define VK_F8                   0x77
	#define VK_F9                   0x78
	#define VK_F10                  0x79
	#define VK_F11                  0x7A
	#define VK_F12                  0x7B
	#define VK_F13                  0x7C
	#define VK_F14                  0x7D
	#define VK_F15                  0x7E
	#define VK_F16                  0x7F
	#define VK_F17                  0x80
	#define VK_F18                  0x81
	#define VK_F19                  0x82
	#define VK_F20                  0x83
	#define VK_F21                  0x84
	#define VK_F22                  0x85
	#define VK_F23                  0x86
	#define VK_F24                  0x87

	#define VK_LSHIFT               0xA0 // Left SHIFT key
	#define VK_RSHIFT               0xA1 // Right SHIFT key
	#define VK_LCONTROL             0xA2 // Left CONTROL key
	#define VK_RCONTROL             0xA3 // Right CONTROL key
	#define VK_LMENU                0xA4 // Left ALT key
	#define VK_RMENU                0xA5 // Right ALT key

#endif