/*
* Key codes for multiple platforms
*
* Copyright (C) 2016 by Sascha Willems - www.saschawillems.de
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#pragma once

#if defined(_WIN32)

/* Mouse buttons */

#define KEY_LBUTTON	0x01 /* Left mouse button */
#define KEY_RBUTTON	0x02 /* Right mouse button */
#define KEY_CANCEL	0x03 /* Control-break processing */
#define KEY_MBUTTON	0x04 /* Middle mouse button (three-button mouse) */
#define KEY_XBUTTON1	0x05 /* Windows 2000/XP: X1 mouse button */
#define KEY_XBUTTON2	0x06 /* Windows 2000/XP: X2 mouse button */

/* 0x07 is undefined */

#define KEY_BACK		0x08 /* BACKSPACE key */
#define KEY_TAB		0x09 /* TAB key */

/* 0x0A to 0x0B are reserved */

#define KEY_CLEAR	0x0C /* CLEAR key */
#define KEY_RETURN	0x0D /* ENTER key */

/* 0x0E to 0x0F are undefined */

#define KEY_SHIFT	0x10 /* SHIFT key */
#define KEY_CONTROL	0x11 /* CTRL key */
#define KEY_MENU		0x12 /* ALT key */
#define KEY_PAUSE	0x13 /* PAUSE key */
#define KEY_CAPITAL	0x14 /* CAPS LOCK key */
#define KEY_KANA		0x15 /* Input Method Editor (IME) Kana mode */
#define KEY_HANGUEL	0x15 /* IME Hanguel mode (maintained for compatibility; use #define KEY_HANGUL) */
#define KEY_HANGUL	0x15 /* IME Hangul mode */

/* 0x16 is undefined */

#define KEY_JUNJA	0x17 /* IME Junja mode */
#define KEY_FINAL	0x18 /* IME final mode */
#define KEY_HANJA	0x19 /* IME Hanja mode */
#define KEY_KANJI	0x19 /* IME Kanji mode */

/* 0x1A is undefined */

#define KEY_ESCAPE	0x1B /* ESC key */
#define KEY_CONVERT	0x1C /* IME convert */
#define KEY_NONCONVERT	0x1D /* IME nonconvert */
#define KEY_ACCEPT	0x1E /* IME accept */
#define KEY_MODECHANGE	0x1F /* IME mode change request */

#define KEY_SPACE	0x20 /* SPACEBAR */
#define KEY_PRIOR	0x21 /* PAGE UP key */
#define KEY_NEXT		0x22 /* PAGE DOWN key */
#define KEY_END		0x23 /* END key */
#define KEY_HOME		0x24 /* HOME key */
#define KEY_LEFT		0x25 /* LEFT ARROW key */
#define KEY_UP		0x26 /* UP ARROW key */
#define KEY_RIGHT	0x27 /* RIGHT ARROW key */
#define KEY_DOWN		0x28 /* DOWN ARROW key */
#define KEY_SELECT	0x29 /* SELECT key */
#define KEY_PRINT	0x2A /* PRINT key */
#define KEY_EXECUTE	0x2B /* EXECUTE key */
#define KEY_SNAPSHOT	0x2C /* PRINT SCREEN key */
#define KEY_INSERT	0x2D /* INS key */
#define KEY_DELETE	0x2E /* DEL key */
#define KEY_HELP		0x2F /* HELP key */

/* Digits, the last 4 bits of the code represent the corresponding digit */

#define KEY_0	0x30 /* '0' key */
#define KEY_1	0x31 /* '1' key */
#define KEY_2	0x32 /* '2' key */
#define KEY_3	0x33 /* '3' key */
#define KEY_4	0x34 /* '4' key */
#define KEY_5	0x35 /* '5' key */
#define KEY_6	0x36 /* '6' key */
#define KEY_7	0x37 /* '7' key */
#define KEY_8	0x38 /* '8' key */
#define KEY_9	0x39 /* '9' key */

/* 0x3A to 0x40 are undefined */

/* The alphabet, the code corresponds to the capitalized letter in the ASCII code */

#define KEY_A	0x41 /* 'A' key */
#define KEY_B	0x42 /* 'B' key */
#define KEY_C	0x43 /* 'C' key */
#define KEY_D	0x44 /* 'D' key */
#define KEY_E	0x45 /* 'E' key */
#define KEY_F	0x46 /* 'F' key */
#define KEY_G	0x47 /* 'G' key */
#define KEY_H	0x48 /* 'H' key */
#define KEY_I	0x49 /* 'I' key */
#define KEY_J	0x4A /* 'J' key */
#define KEY_K	0x4B /* 'K' key */
#define KEY_L	0x4C /* 'L' key */
#define KEY_M	0x4D /* 'M' key */
#define KEY_N	0x4E /* 'N' key */
#define KEY_O	0x4F /* 'O' key */
#define KEY_P	0x50 /* 'P' key */
#define KEY_Q	0x51 /* 'Q' key */
#define KEY_R	0x52 /* 'R' key */
#define KEY_S	0x53 /* 'S' key */
#define KEY_T	0x54 /* 'T' key */
#define KEY_U	0x55 /* 'U' key */
#define KEY_V	0x56 /* 'V' key */
#define KEY_W	0x57 /* 'W' key */
#define KEY_X	0x58 /* 'X' key */
#define KEY_Y	0x59 /* 'Y' key */
#define KEY_Z	0x5A /* 'Z' key */

#define KEY_LWIN		0x5B /* Left Windows key (Microsoft Natural keyboard) */
#define KEY_RWIN		0x5C /* Right Windows key (Natural keyboard) */
#define KEY_APPS		0x5D /* Applications key (Natural keyboard) */

/* 0x5E is reserved */

#define KEY_SLEEP	0x5F /* Computer Sleep key */

/* Numeric keypad digits, the last four bits of the code represent the corresponding digit */

#define KEY_NUMPAD0	0x60 /* Numeric keypad '0' key */
#define KEY_NUMPAD1	0x61 /* Numeric keypad '1' key */
#define KEY_NUMPAD2	0x62 /* Numeric keypad '2' key */
#define KEY_NUMPAD3	0x63 /* Numeric keypad '3' key */
#define KEY_NUMPAD4	0x64 /* Numeric keypad '4' key */
#define KEY_NUMPAD5	0x65 /* Numeric keypad '5' key */
#define KEY_NUMPAD6	0x66 /* Numeric keypad '6' key */
#define KEY_NUMPAD7	0x67 /* Numeric keypad '7' key */
#define KEY_NUMPAD8	0x68 /* Numeric keypad '8' key */
#define KEY_NUMPAD9	0x69 /* Numeric keypad '9' key */

/* Numeric keypad operators and special keys */

#define KEY_MULTIPLY	0x6A /* Multiply key */
#define KEY_ADD		0x6B /* Add key */
#define KEY_SEPARATOR	0x6C /* Separator key */
#define KEY_SUBTRACT	0x6D /* Subtract key */
#define KEY_DECIMAL	0x6E /* Decimal key */
#define KEY_DIVIDE	0x6F /* Divide key */

/* Function keys, from F1 to F24 */

#define KEY_F1		0x70 /* F1 key */
#define KEY_F2		0x71 /* F2 key */
#define KEY_F3		0x72 /* F3 key */
#define KEY_F4		0x73 /* F4 key */
#define KEY_F5		0x74 /* F5 key */
#define KEY_F6		0x75 /* F6 key */
#define KEY_F7		0x76 /* F7 key */
#define KEY_F8		0x77 /* F8 key */
#define KEY_F9		0x78 /* F9 key */
#define KEY_F10		0x79 /* F10 key */
#define KEY_F11		0x7A /* F11 key */
#define KEY_F12		0x7B /* F12 key */
#define KEY_F13		0x7C /* F13 key */
#define KEY_F14		0x7D /* F14 key */
#define KEY_F15		0x7E /* F15 key */
#define KEY_F16		0x7F /* F16 key */
#define KEY_F17		0x80 /* F17 key */
#define KEY_F18		0x81 /* F18 key */
#define KEY_F19		0x82 /* F19 key */
#define KEY_F20		0x83 /* F20 key */
#define KEY_F21		0x84 /* F21 key */
#define KEY_F22		0x85 /* F22 key */
#define KEY_F23		0x86 /* F23 key */
#define KEY_F24		0x87 /* F24 key */

/* 0x88 to 0x8F are unassigned */

#define KEY_NUMLOCK	0x90 /* NUM LOCK key */
#define KEY_SCROLL	0x91 /* SCROLL LOCK key */

/* 0x92 to 0x96 are OEM specific */
/* 0x97 to 0x9F are unassigned */

/* Modifier keys */

#define KEY_LSHIFT	0xA0 /* Left SHIFT key */
#define KEY_RSHIFT	0xA1 /* Right SHIFT key */
#define KEY_LCONTROL	0xA2 /* Left CONTROL key */
#define KEY_RCONTROL	0xA3 /* Right CONTROL key */
#define KEY_LMENU	0xA4 /* Left MENU key */
#define KEY_RMENU	0xA5 /* Right MENU key */

/* Browser related keys */

#define KEY_BROWSER_BACK		0xA6 /* Windows 2000/XP: Browser Back key */
#define KEY_BROWSER_FORWARD	0xA7 /* Windows 2000/XP: Browser Forward key */
#define KEY_BROWSER_REFRESH	0xA8 /* Windows 2000/XP: Browser Refresh key */
#define KEY_BROWSER_STOP		0xA9 /* Windows 2000/XP: Browser Stop key */
#define KEY_BROWSER_SEARCH	0xAA /* Windows 2000/XP: Browser Search key */
#define KEY_BROWSER_FAVORITES	0xAB /* Windows 2000/XP: Browser Favorites key */
#define KEY_BROWSER_HOME		0xAC /* Windows 2000/XP: Browser Start and Home key */

/* Volume related keys */

#define KEY_VOLUME_MUTE		0xAD /* Windows 2000/XP: Volume Mute key */
#define KEY_VOLUME_DOWN		0xAE /* Windows 2000/XP: Volume Down key */
#define KEY_VOLUME_UP		0xAF /* Windows 2000/XP: Volume Up key */

/* Media player related keys */

#define KEY_MEDIA_NEXT_TRACK	0xB0 /* Windows 2000/XP: Next Track key */
#define KEY_MEDIA_PREV_TRACK	0xB1 /* Windows 2000/XP: Previous Track key */
#define KEY_MEDIA_STOP		0xB2 /* Windows 2000/XP: Stop Media key */
#define KEY_MEDIA_PLAY_PAUSE	0xB3 /* Windows 2000/XP: Play/Pause Media key */

/* Application launcher keys */

#define KEY_LAUNCH_MAIL		0xB4 /* Windows 2000/XP: Start Mail key */
#define KEY_MEDIA_SELECT		0xB5 /* Windows 2000/XP: Select Media key */
#define KEY_LAUNCH_APP1		0xB6 /* Windows 2000/XP: Start Application 1 key */
#define KEY_LAUNCH_APP2		0xB7 /* Windows 2000/XP: Start Application 2 key */

/* 0xB8 and 0xB9 are reserved */

/* OEM keys */

#define KEY_OEM_1	0xBA /* Used for miscellaneous characters; it can vary by keyboard. */
			     /* Windows 2000/XP: For the US standard keyboard, the ';:' key */

#define KEY_OEM_PLUS	0xBB /* Windows 2000/XP: For any country/region, the '+' key */
#define KEY_OEM_COMMA	0xBC /* Windows 2000/XP: For any country/region, the ',' key */
#define KEY_OEM_MINUS	0xBD /* Windows 2000/XP: For any country/region, the '-' key */
#define KEY_OEM_PERIOD	0xBE /* Windows 2000/XP: For any country/region, the '.' key */

#define KEY_OEM_2	0xBF /* Used for miscellaneous characters; it can vary by keyboard. */
			     /* Windows 2000/XP: For the US standard keyboard, the '/?' key */

#define KEY_OEM_3	0xC0 /* Used for miscellaneous characters; it can vary by keyboard. */
			     /* Windows 2000/XP: For the US standard keyboard, the '`~' key */

/* 0xC1 to 0xD7 are reserved */
#define KEY_ABNT_C1	0xC1 /* Brazilian (ABNT) Keyboard */
#define KEY_ABNT_C2	0xC2 /* Brazilian (ABNT) Keyboard */

/* 0xD8 to 0xDA are unassigned */

#define KEY_OEM_4	0xDB /* Used for miscellaneous characters; it can vary by keyboard. */
			     /* Windows 2000/XP: For the US standard keyboard, the '[{' key */

#define KEY_OEM_5	0xDC /* Used for miscellaneous characters; it can vary by keyboard. */
			     /* Windows 2000/XP: For the US standard keyboard, the '\|' key */

#define KEY_OEM_6	0xDD /* Used for miscellaneous characters; it can vary by keyboard. */
			     /* Windows 2000/XP: For the US standard keyboard, the ']}' key */

#define KEY_OEM_7	0xDE /* Used for miscellaneous characters; it can vary by keyboard. */
			     /* Windows 2000/XP: For the US standard keyboard, the 'single-quote/double-quote' key */

#define KEY_OEM_8	0xDF /* Used for miscellaneous characters; it can vary by keyboard. */

/* 0xE0 is reserved */
/* 0xE1 is OEM specific */

#define KEY_OEM_102	0xE2 /* Windows 2000/XP: Either the angle bracket key or */
			     /* the backslash key on the RT 102-key keyboard */

/* 0xE3 and 0xE4 are OEM specific */

#define KEY_PROCESSKEY	0xE5 /* Windows 95/98/Me, Windows NT 4.0, Windows 2000/XP: IME PROCESS key */

/* 0xE6 is OEM specific */

#define KEY_PACKET	0xE7	/* Windows 2000/XP: Used to pass Unicode characters as if they were keystrokes. */
				/* The #define KEY_PACKET key is the low word of a 32-bit Virtual Key value used */
				/* for non-keyboard input methods. For more information, */
				/* see Remark in KEYBDINPUT, SendInput, WM_KEYDOWN, and WM_KEYUP */

/* 0xE8 is unassigned */
/* 0xE9 to 0xF5 are OEM specific */

#define KEY_ATTN		0xF6 /* Attn key */
#define KEY_CRSEL	0xF7 /* CrSel key */
#define KEY_EXSEL	0xF8 /* ExSel key */
#define KEY_EREOF	0xF9 /* Erase EOF key */
#define KEY_PLAY		0xFA /* Play key */
#define KEY_ZOOM		0xFB /* Zoom key */
#define KEY_NONAME	0xFC /* Reserved */
#define KEY_PA1		0xFD /* PA1 key */
#define KEY_OEM_CLEAR	0xFE /* Clear key */

#elif defined(KEY_USE_PLATFORM_ANDROID_KHR)
#define GAMEPAD_BUTTON_A 0x1000
#define GAMEPAD_BUTTON_B 0x1001
#define GAMEPAD_BUTTON_X 0x1002
#define GAMEPAD_BUTTON_Y 0x1003
#define GAMEPAD_BUTTON_L1 0x1004
#define GAMEPAD_BUTTON_R1 0x1005
#define GAMEPAD_BUTTON_START 0x1006
#define TOUCH_DOUBLE_TAP 0x1100

#elif defined(KEY_USE_PLATFORM_IOS_MVK)
// Use numeric keys instead of function keys.
// Use main keyboard plus/minus instead of keypad plus/minus
// Use Delete key instead of Escape key.
#define KEY_ESCAPE 0x33
#define KEY_F1 '1'
#define KEY_F2 '2'
#define KEY_F3 '3'
#define KEY_F4 '4'
#define KEY_W 'w'
#define KEY_A 'a'
#define KEY_S 's'
#define KEY_D 'd'
#define KEY_P 'p'
#define KEY_SPACE ' '
#define KEY_KPADD '+'
#define KEY_KPSUB '-'
#define KEY_B 'b'
#define KEY_F 'f'
#define KEY_L 'l'
#define KEY_N 'n'
#define KEY_O 'o'
#define KEY_T 't'

#elif defined(KEY_USE_PLATFORM_MACOS_MVK)
// For compatibility with iOS UX and absent keypad on MacBook:
// - Use numeric keys instead of function keys
// - Use main keyboard plus/minus instead of keypad plus/minus
// - Use Delete key instead of Escape key
#define KEY_ESCAPE 0x33
#define KEY_F1 0x12
#define KEY_F2 0x13
#define KEY_F3 0x14
#define KEY_F4 0x15
#define KEY_W 0x0D
#define KEY_A 0x00
#define KEY_S 0x01
#define KEY_D 0x02
#define KEY_P 0x23
#define KEY_SPACE 0x31
#define KEY_KPADD 0x18
#define KEY_KPSUB 0x1B
#define KEY_B 0x0B
#define KEY_F 0x03
#define KEY_L 0x25
#define KEY_N 0x2D
#define KEY_O 0x1F
#define KEY_T 0x11

#elif defined(KEY_USE_PLATFORM_DIRECTFB_EXT)
#define KEY_ESCAPE DIKS_ESCAPE
#define KEY_F1 DIKS_F1
#define KEY_F2 DIKS_F2
#define KEY_F3 DIKS_F3
#define KEY_F4 DIKS_F4
#define KEY_W DIKS_SMALL_W
#define KEY_A DIKS_SMALL_A
#define KEY_S DIKS_SMALL_S
#define KEY_D DIKS_SMALL_D
#define KEY_P DIKS_SMALL_P
#define KEY_SPACE DIKS_SPACE
#define KEY_KPADD DIKS_PLUS_SIGN
#define KEY_KPSUB DIKS_MINUS_SIGN
#define KEY_B DIKS_SMALL_B
#define KEY_F DIKS_SMALL_F
#define KEY_L DIKS_SMALL_L
#define KEY_N DIKS_SMALL_N
#define KEY_O DIKS_SMALL_O
#define KEY_T DIKS_SMALL_T

#elif defined(KEY_USE_PLATFORM_WAYLAND_KHR)
#include <linux/input.h>

// todo: hack for bloom example
#define KEY_KPADD KEY_KPPLUS
#define KEY_KPSUB KEY_KPMINUS

#elif defined(__linux__) || defined(__FreeBSD__)

#include <linux/input-event-codes.h>

#define KEY_NUMPAD0 KEY_KP0
#define KEY_NUMPAD1 KEY_KP1
#define KEY_NUMPAD2 KEY_KP2
#define KEY_NUMPAD3 KEY_KP3
#define KEY_NUMPAD4 KEY_KP4
#define KEY_NUMPAD5 KEY_KP5
#define KEY_NUMPAD6 KEY_KP6
#define KEY_NUMPAD7 KEY_KP7
#define KEY_NUMPAD8 KEY_KP8
#define KEY_NUMPAD9 KEY_KP9
#define KEY_NUMPAD_DIVIDE KEY_KPDIV
#define KEY_NUMPAD_MULTIPLY KEY_KPMULT
#define KEY_NUMPAD_SUBTRACT KEY_KPSUB
#define KEY_NUMPAD_ADD KEY_KPADD
#define KEY_NUMPAD_ENTER KEY_KPENTER
#define KEY_NUMPAD_DECIMAL KEY_KPDECIMAL
#define KEY_NUMPAD_EQUAL KEY_KPEQUAL
#define KEY_ESCAPE KEY_ESC
#define KEY_PRIOR KEY_PAGEUP


#endif
