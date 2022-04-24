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

#define KEY_RESERVED		0
#define KEY_ESC			1
#define KEY_1			2
#define KEY_2			3
#define KEY_3			4
#define KEY_4			5
#define KEY_5			6
#define KEY_6			7
#define KEY_7			8
#define KEY_8			9
#define KEY_9			10
#define KEY_0			11
#define KEY_MINUS		12
#define KEY_EQUAL		13
#define KEY_BACKSPACE		14
#define KEY_TAB			15
#define KEY_Q			16
#define KEY_W			17
#define KEY_E			18
#define KEY_R			19
#define KEY_T			20
#define KEY_Y			21
#define KEY_U			22
#define KEY_I			23
#define KEY_O			24
#define KEY_P			25
#define KEY_LEFTBRACE		26
#define KEY_RIGHTBRACE		27
#define KEY_ENTER		28
#define KEY_LEFTCTRL		29
#define KEY_A			30
#define KEY_S			31
#define KEY_D			32
#define KEY_F			33
#define KEY_G			34
#define KEY_H			35
#define KEY_J			36
#define KEY_K			37
#define KEY_L			38
#define KEY_SEMICOLON		39
#define KEY_APOSTROPHE		40
#define KEY_GRAVE		41
#define KEY_LEFTSHIFT		42
#define KEY_BACKSLASH		43
#define KEY_Z			44
#define KEY_X			45
#define KEY_C			46
#define KEY_V			47
#define KEY_B			48
#define KEY_N			49
#define KEY_M			50
#define KEY_COMMA		51
#define KEY_DOT			52
#define KEY_SLASH		53
#define KEY_RIGHTSHIFT		54
#define KEY_KPASTERISK		55
#define KEY_LEFTALT		56
#define KEY_SPACE		57
#define KEY_CAPSLOCK		58
#define KEY_F1			59
#define KEY_F2			60
#define KEY_F3			61
#define KEY_F4			62
#define KEY_F5			63
#define KEY_F6			64
#define KEY_F7			65
#define KEY_F8			66
#define KEY_F9			67
#define KEY_F10			68
#define KEY_NUMLOCK		69
#define KEY_SCROLLLOCK		70
#define KEY_KP7			71
#define KEY_KP8			72
#define KEY_KP9			73
#define KEY_KPMINUS		74
#define KEY_KP4			75
#define KEY_KP5			76
#define KEY_KP6			77
#define KEY_KPPLUS		78
#define KEY_KP1			79
#define KEY_KP2			80
#define KEY_KP3			81
#define KEY_KP0			82
#define KEY_KPDOT		83
#define KEY_ZENKAKUHANKAKU	85
#define KEY_102ND		86
#define KEY_F11			87
#define KEY_F12			88
#define KEY_RO			89
#define KEY_KATAKANA		90
#define KEY_HIRAGANA		91
#define KEY_HENKAN		92
#define KEY_KATAKANAHIRAGANA	93
#define KEY_MUHENKAN		94
#define KEY_KPJPCOMMA		95
#define KEY_KPENTER		96
#define KEY_RIGHTCTRL		97
#define KEY_KPSLASH		98
#define KEY_SYSRQ		99
#define KEY_RIGHTALT		100
#define KEY_LINEFEED		101
#define KEY_HOME		102
#define KEY_UP			103
#define KEY_PAGEUP		104
#define KEY_LEFT		105
#define KEY_RIGHT		106
#define KEY_END			107
#define KEY_DOWN		108
#define KEY_PAGEDOWN		109
#define KEY_INSERT		110
#define KEY_DELETE		111
#define KEY_MACRO		112
#define KEY_MUTE		113
#define KEY_VOLUMEDOWN		114
#define KEY_VOLUMEUP		115
#define KEY_POWER		116	/* SC System Power Down */
#define KEY_KPEQUAL		117
#define KEY_KPPLUSMINUS		118
#define KEY_PAUSE		119
#define KEY_SCALE		120	/* AL Compiz Scale (Expose) */
#define KEY_KPCOMMA		121
#define KEY_HANGEUL		122
#define KEY_HANGUEL		KEY_HANGEUL
#define KEY_HANJA		123
#define KEY_YEN			124
#define KEY_LEFTMETA		125
#define KEY_RIGHTMETA		126
#define KEY_COMPOSE		127
#define KEY_STOP		128	/* AC Stop */
#define KEY_AGAIN		129
#define KEY_PROPS		130	/* AC Properties */
#define KEY_UNDO		131	/* AC Undo */
#define KEY_FRONT		132
#define KEY_COPY		133	/* AC Copy */
#define KEY_OPEN		134	/* AC Open */
#define KEY_PASTE		135	/* AC Paste */
#define KEY_FIND		136	/* AC Search */
#define KEY_CUT			137	/* AC Cut */
#define KEY_HELP		138	/* AL Integrated Help Center */
#define KEY_MENU		139	/* Menu (show menu) */
#define KEY_CALC		140	/* AL Calculator */
#define KEY_SETUP		141
#define KEY_SLEEP		142	/* SC System Sleep */
#define KEY_WAKEUP		143	/* System Wake Up */
#define KEY_FILE		144	/* AL Local Machine Browser */
#define KEY_SENDFILE		145
#define KEY_DELETEFILE		146
#define KEY_XFER		147
#define KEY_PROG1		148
#define KEY_PROG2		149
#define KEY_WWW			150	/* AL Internet Browser */
#define KEY_MSDOS		151
#define KEY_COFFEE		152	/* AL Terminal Lock/Screensaver */
#define KEY_SCREENLOCK		KEY_COFFEE
#define KEY_DIRECTION		153
#define KEY_CYCLEWINDOWS	154
#define KEY_MAIL		155
#define KEY_BOOKMARKS		156	/* AC Bookmarks */
#define KEY_COMPUTER		157
#define KEY_BACK		158	/* AC Back */
#define KEY_FORWARD		159	/* AC Forward */
#define KEY_CLOSECD		160
#define KEY_EJECTCD		161
#define KEY_EJECTCLOSECD	162
#define KEY_NEXTSONG		163
#define KEY_PLAYPAUSE		164
#define KEY_PREVIOUSSONG	165
#define KEY_STOPCD		166
#define KEY_RECORD		167
#define KEY_REWIND		168
#define KEY_PHONE		169	/* Media Select Telephone */
#define KEY_ISO			170
#define KEY_CONFIG		171	/* AL Consumer Control Configuration */
#define KEY_HOMEPAGE		172	/* AC Home */
#define KEY_REFRESH		173	/* AC Refresh */
#define KEY_EXIT		174	/* AC Exit */
#define KEY_MOVE		175
#define KEY_EDIT		176
#define KEY_SCROLLUP		177
#define KEY_SCROLLDOWN		178
#define KEY_KPLEFTPAREN		179
#define KEY_KPRIGHTPAREN	180
#define KEY_NEW			181	/* AC New */
#define KEY_REDO		182	/* AC Redo/Repeat */
#define KEY_F13			183
#define KEY_F14			184
#define KEY_F15			185
#define KEY_F16			186
#define KEY_F17			187
#define KEY_F18			188
#define KEY_F19			189
#define KEY_F20			190
#define KEY_F21			191
#define KEY_F22			192
#define KEY_F23			193
#define KEY_F24			194
#define KEY_PLAYCD		200
#define KEY_PAUSECD		201
#define KEY_PROG3		202
#define KEY_PROG4		203
#define KEY_DASHBOARD		204	/* AL Dashboard */
#define KEY_SUSPEND		205
#define KEY_CLOSE		206	/* AC Close */
#define KEY_PLAY		207
#define KEY_FASTFORWARD		208
#define KEY_BASSBOOST		209
#define KEY_PRINT		210	/* AC Print */
#define KEY_HP			211
#define KEY_CAMERA		212
#define KEY_SOUND		213
#define KEY_QUESTION		214
#define KEY_EMAIL		215
#define KEY_CHAT		216
#define KEY_SEARCH		217
#define KEY_CONNECT		218
#define KEY_FINANCE		219	/* AL Checkbook/Finance */
#define KEY_SPORT		220
#define KEY_SHOP		221
#define KEY_ALTERASE		222
#define KEY_CANCEL		223	/* AC Cancel */
#define KEY_BRIGHTNESSDOWN	224
#define KEY_BRIGHTNESSUP	225
#define KEY_MEDIA		226
#define KEY_SWITCHVIDEOMODE	227	/* Cycle between available video
					   outputs (Monitor/LCD/TV-out/etc) */
#define KEY_KBDILLUMTOGGLE	228
#define KEY_KBDILLUMDOWN	229
#define KEY_KBDILLUMUP		230
#define KEY_SEND		231	/* AC Send */
#define KEY_REPLY		232	/* AC Reply */
#define KEY_FORWARDMAIL		233	/* AC Forward Msg */
#define KEY_SAVE		234	/* AC Save */
#define KEY_DOCUMENTS		235
#define KEY_BATTERY		236
#define KEY_BLUETOOTH		237
#define KEY_WLAN		238
#define KEY_UWB			239
#define KEY_UNKNOWN		240
#define KEY_VIDEO_NEXT		241	/* drive next video source */
#define KEY_VIDEO_PREV		242	/* drive previous video source */
#define KEY_BRIGHTNESS_CYCLE	243	/* brightness up, after max is min */
#define KEY_BRIGHTNESS_ZERO	244	/* brightness off, use ambient */
#define KEY_DISPLAY_OFF		245	/* display device to off state */
#define KEY_WIMAX		246
#define KEY_RFKILL		247	/* Key that controls all radios */
#define KEY_MICMUTE		248	/* Mute / unmute the microphone */
/* Code 255 is reserved for special needs of AT keyboard driver */
#define BTN_MISC		0x100
#define BTN_0			0x100
#define BTN_1			0x101
#define BTN_2			0x102
#define BTN_3			0x103
#define BTN_4			0x104
#define BTN_5			0x105
#define BTN_6			0x106
#define BTN_7			0x107
#define BTN_8			0x108
#define BTN_9			0x109
#define BTN_MOUSE		0x110
#define BTN_LEFT		0x110
#define BTN_RIGHT		0x111
#define BTN_MIDDLE		0x112
#define BTN_SIDE		0x113
#define BTN_EXTRA		0x114
#define BTN_FORWARD		0x115
#define BTN_BACK		0x116
#define BTN_TASK		0x117
#define BTN_JOYSTICK		0x120
#define BTN_TRIGGER		0x120
#define BTN_THUMB		0x121
#define BTN_THUMB2		0x122
#define BTN_TOP			0x123
#define BTN_TOP2		0x124
#define BTN_PINKIE		0x125
#define BTN_BASE		0x126
#define BTN_BASE2		0x127
#define BTN_BASE3		0x128
#define BTN_BASE4		0x129
#define BTN_BASE5		0x12a
#define BTN_BASE6		0x12b
#define BTN_DEAD		0x12f
#define BTN_GAMEPAD		0x130
#define BTN_A			0x130
#define BTN_B			0x131
#define BTN_C			0x132
#define BTN_X			0x133
#define BTN_Y			0x134
#define BTN_Z			0x135
#define BTN_TL			0x136
#define BTN_TR			0x137
#define BTN_TL2			0x138
#define BTN_TR2			0x139
#define BTN_SELECT		0x13a
#define BTN_START		0x13b
#define BTN_MODE		0x13c
#define BTN_THUMBL		0x13d
#define BTN_THUMBR		0x13e
#define BTN_DIGI		0x140
#define BTN_TOOL_PEN		0x140
#define BTN_TOOL_RUBBER		0x141
#define BTN_TOOL_BRUSH		0x142
#define BTN_TOOL_PENCIL		0x143
#define BTN_TOOL_AIRBRUSH	0x144
#define BTN_TOOL_FINGER		0x145
#define BTN_TOOL_MOUSE		0x146
#define BTN_TOOL_LENS		0x147
#define BTN_TOOL_QUINTTAP	0x148	/* Five fingers on trackpad */
#define BTN_TOUCH		0x14a
#define BTN_STYLUS		0x14b
#define BTN_STYLUS2		0x14c
#define BTN_TOOL_DOUBLETAP	0x14d
#define BTN_TOOL_TRIPLETAP	0x14e
#define BTN_TOOL_QUADTAP	0x14f	/* Four fingers on trackpad */
#define BTN_WHEEL		0x150
#define BTN_GEAR_DOWN		0x150
#define BTN_GEAR_UP		0x151
#define KEY_OK			0x160
#define KEY_SELECT		0x161
#define KEY_GOTO		0x162
#define KEY_CLEAR		0x163
#define KEY_POWER2		0x164
#define KEY_OPTION		0x165
#define KEY_INFO		0x166	/* AL OEM Features/Tips/Tutorial */
#define KEY_TIME		0x167
#define KEY_VENDOR		0x168
#define KEY_ARCHIVE		0x169
#define KEY_PROGRAM		0x16a	/* Media Select Program Guide */
#define KEY_CHANNEL		0x16b
#define KEY_FAVORITES		0x16c
#define KEY_EPG			0x16d
#define KEY_PVR			0x16e	/* Media Select Home */
#define KEY_MHP			0x16f
#define KEY_LANGUAGE		0x170
#define KEY_TITLE		0x171
#define KEY_SUBTITLE		0x172
#define KEY_ANGLE		0x173
#define KEY_ZOOM		0x174
#define KEY_MODE		0x175
#define KEYBOARD		0x176
#define KEY_SCREEN		0x177
#define KEY_PC			0x178	/* Media Select Computer */
#define KEY_TV			0x179	/* Media Select TV */
#define KEY_TV2			0x17a	/* Media Select Cable */
#define KEY_VCR			0x17b	/* Media Select VCR */
#define KEY_VCR2		0x17c	/* VCR Plus */
#define KEY_SAT			0x17d	/* Media Select Satellite */
#define KEY_SAT2		0x17e
#define KEY_CD			0x17f	/* Media Select CD */
#define KEY_TAPE		0x180	/* Media Select Tape */
#define KEY_RADIO		0x181
#define KEY_TUNER		0x182	/* Media Select Tuner */
#define KEY_PLAYER		0x183
#define KEY_TEXT		0x184
#define KEY_DVD			0x185	/* Media Select DVD */
#define KEY_AUX			0x186
#define KEY_MP3			0x187
#define KEY_AUDIO		0x188	/* AL Audio Browser */
#define KEY_VIDEO		0x189	/* AL Movie Browser */
#define KEY_DIRECTORY		0x18a
#define KEY_LIST		0x18b
#define KEY_MEMO		0x18c	/* Media Select Messages */
#define KEY_CALENDAR		0x18d
#define KEY_RED			0x18e
#define KEY_GREEN		0x18f
#define KEY_YELLOW		0x190
#define KEY_BLUE		0x191
#define KEY_CHANNELUP		0x192	/* Channel Increment */
#define KEY_CHANNELDOWN		0x193	/* Channel Decrement */
#define KEY_FIRST		0x194
#define KEY_LAST		0x195	/* Recall Last */
#define KEY_AB			0x196
#define KEY_NEXT		0x197
#define KEY_RESTART		0x198
#define KEY_SLOW		0x199
#define KEY_SHUFFLE		0x19a
#define KEY_BREAK		0x19b
#define KEY_PREVIOUS		0x19c
#define KEY_DIGITS		0x19d
#define KEY_TEEN		0x19e
#define KEY_TWEN		0x19f
#define KEY_VIDEOPHONE		0x1a0	/* Media Select Video Phone */
#define KEY_GAMES		0x1a1	/* Media Select Games */
#define KEY_ZOOMIN		0x1a2	/* AC Zoom In */
#define KEY_ZOOMOUT		0x1a3	/* AC Zoom Out */
#define KEY_ZOOMRESET		0x1a4	/* AC Zoom */
#define KEY_WORDPROCESSOR	0x1a5	/* AL Word Processor */
#define KEY_EDITOR		0x1a6	/* AL Text Editor */
#define KEY_SPREADSHEET		0x1a7	/* AL Spreadsheet */
#define KEY_GRAPHICSEDITOR	0x1a8	/* AL Graphics Editor */
#define KEY_PRESENTATION	0x1a9	/* AL Presentation App */
#define KEY_DATABASE		0x1aa	/* AL Database App */
#define KEY_NEWS		0x1ab	/* AL Newsreader */
#define KEY_VOICEMAIL		0x1ac	/* AL Voicemail */
#define KEY_ADDRESSBOOK		0x1ad	/* AL Contacts/Address Book */
#define KEY_MESSENGER		0x1ae	/* AL Instant Messaging */
#define KEY_DISPLAYTOGGLE	0x1af	/* Turn display (LCD) on and off */
#define KEY_SPELLCHECK		0x1b0   /* AL Spell Check */
#define KEY_LOGOFF		0x1b1   /* AL Logoff */
#define KEY_DOLLAR		0x1b2
#define KEY_EURO		0x1b3
#define KEY_FRAMEBACK		0x1b4	/* Consumer - transport controls */
#define KEY_FRAMEFORWARD	0x1b5
#define KEY_CONTEXT_MENU	0x1b6	/* GenDesc - system context menu */
#define KEY_MEDIA_REPEAT	0x1b7	/* Consumer - transport control */
#define KEY_10CHANNELSUP	0x1b8	/* 10 channels up (10+) */
#define KEY_10CHANNELSDOWN	0x1b9	/* 10 channels down (10-) */
#define KEY_IMAGES		0x1ba	/* AL Image Browser */
#define KEY_DEL_EOL		0x1c0
#define KEY_DEL_EOS		0x1c1
#define KEY_INS_LINE		0x1c2
#define KEY_DEL_LINE		0x1c3
#define KEY_FN			0x1d0
#define KEY_FN_ESC		0x1d1
#define KEY_FN_F1		0x1d2
#define KEY_FN_F2		0x1d3
#define KEY_FN_F3		0x1d4
#define KEY_FN_F4		0x1d5
#define KEY_FN_F5		0x1d6
#define KEY_FN_F6		0x1d7
#define KEY_FN_F7		0x1d8
#define KEY_FN_F8		0x1d9
#define KEY_FN_F9		0x1da
#define KEY_FN_F10		0x1db
#define KEY_FN_F11		0x1dc
#define KEY_FN_F12		0x1dd
#define KEY_FN_1		0x1de
#define KEY_FN_2		0x1df
#define KEY_FN_D		0x1e0
#define KEY_FN_E		0x1e1
#define KEY_FN_F		0x1e2
#define KEY_FN_S		0x1e3
#define KEY_FN_B		0x1e4
#define KEY_BRL_DOT1		0x1f1
#define KEY_BRL_DOT2		0x1f2
#define KEY_BRL_DOT3		0x1f3
#define KEY_BRL_DOT4		0x1f4
#define KEY_BRL_DOT5		0x1f5
#define KEY_BRL_DOT6		0x1f6
#define KEY_BRL_DOT7		0x1f7
#define KEY_BRL_DOT8		0x1f8
#define KEY_BRL_DOT9		0x1f9
#define KEY_BRL_DOT10		0x1fa
#define KEY_NUMERIC_0		0x200	/* used by phones, remote controls, */
#define KEY_NUMERIC_1		0x201	/* and other keypads */
#define KEY_NUMERIC_2		0x202
#define KEY_NUMERIC_3		0x203
#define KEY_NUMERIC_4		0x204
#define KEY_NUMERIC_5		0x205
#define KEY_NUMERIC_6		0x206
#define KEY_NUMERIC_7		0x207
#define KEY_NUMERIC_8		0x208
#define KEY_NUMERIC_9		0x209
#define KEY_NUMERIC_STAR	0x20a
#define KEY_NUMERIC_POUND	0x20b
#define KEY_CAMERA_SNAPSHOT	0x2fe
#define KEY_CAMERA_FOCUS	0x210
#define KEY_WPS_BUTTON		0x211	/* WiFi Protected Setup key */
#define KEY_TOUCHPAD_TOGGLE	0x212	/* Request switch touchpad on or off */
#define KEY_TOUCHPAD_ON		0x213
#define KEY_TOUCHPAD_OFF	0x214
#define KEY_CAMERA_ZOOMIN	0x215
#define KEY_CAMERA_ZOOMOUT	0x216
#define KEY_CAMERA_UP		0x217
#define KEY_CAMERA_DOWN		0x218
#define KEY_CAMERA_LEFT		0x219
#define KEY_CAMERA_RIGHT	0x21a
#endif