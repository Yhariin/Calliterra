#pragma once
#include "pch.h"

typedef unsigned char KeyCode;

static std::string KeyCodeToString(KeyCode key)
{
	switch (key)
	{
	case VK_LBUTTON:
		return "LEFT MOUSE BUTTON";
	case VK_RBUTTON:
		return "RIGHT MOUSE BUTTON";
	case VK_CANCEL:
		return "BREAK";
	case VK_MBUTTON:
		return "MIDDLE MOUSE BUTTON";
	case VK_XBUTTON1:
		return "BACK MOUSE BUTTON";
	case VK_XBUTTON2:
		return "FORWARD MOUSE BUTTON";
	case VK_BACK:
		return "BACKSPACE";
	case VK_TAB:
		return "TAB";
	case VK_CLEAR:
		return "CLEAR";
	case VK_RETURN:
		return "ENTER";
	case VK_SHIFT:
		return "SHIFT";
	case VK_CONTROL:
		return "CTRL";
	case VK_MENU:
		return "ALT";
	case VK_PAUSE:
		return "PAUSE";
	case VK_CAPITAL:
		return "CAPSLOCK";
	case VK_ESCAPE:
		return "ESC";
	case VK_SPACE:
		return "SPACE";
	case VK_PRIOR:
		return "PAGE UP";
	case VK_NEXT:
		return "PAGE DOWN";
	case VK_END:
		return "END";
	case VK_HOME:
		return "HOME";
	case VK_LEFT:
		return "LEFT ARROW";
	case VK_UP:
		return "UP ARROW";
	case VK_RIGHT:
		return "RIGHT ARROW";
	case VK_DOWN:
		return "DOWN ARROW";
	case VK_SELECT:
		return "SELECT";
	case VK_PRINT:
		return "PRINT";
	case VK_EXECUTE:
		return "EXECUTE";
	case VK_SNAPSHOT:
		return "PRINT SCREEN";
	case VK_INSERT:
		return "INS";
	case VK_DELETE:
		return "DEL";
	case VK_HELP:
		return "HELP";
	case VK_LWIN:
		return "WINDOWS";
	case VK_RWIN:
		return "WINDOWS";
	case VK_APPS:
		return "APPLICATIONS";
	case VK_SLEEP:
		return "SLEEP";
	case VK_NUMPAD0:
		return "NUMPAD 0";
	case VK_NUMPAD1:
		return "NUMPAD 1";
	case VK_NUMPAD2:
		return "NUMPAD 2";
	case VK_NUMPAD3:
		return "NUMPAD 3";
	case VK_NUMPAD4:
		return "NUMPAD 4";
	case VK_NUMPAD5:
		return "NUMPAD 5";
	case VK_NUMPAD6:
		return "NUMPAD 6";
	case VK_NUMPAD7:
		return "NUMPAD 7";
	case VK_NUMPAD8:
		return "NUMPAD 8";
	case VK_NUMPAD9:
		return "NUMPAD 9";
	case VK_MULTIPLY:
		return "MULTIPLY";
	case VK_ADD:
		return "ADD";
	case VK_SEPARATOR:
		return "SEPARATOR";
	case VK_SUBTRACT:
		return "SUBTRACT";
	case VK_DECIMAL:
		return "DECIMAL";
	case VK_DIVIDE:
		return "DIVIDE";
	case VK_F1:
		return "F1";
	case VK_F2:
		return "F2";
	case VK_F3:
		return "F3";
	case VK_F4:
		return "F4";
	case VK_F5:
		return "F5";
	case VK_F6:
		return "F6";
	case VK_F7:
		return "F8";
	case VK_F9:
		return "F9";
	case VK_F10:
		return "F10";
	case VK_F11:
		return "F11";
	case VK_F12:
		return "F12";
	case VK_F13:
		return "F13";
	case VK_F14:
		return "F14";
	case VK_F15:
		return "F15";
	case VK_F16:
		return "F16";
	case VK_F17:
		return "F17";
	case VK_F18:
		return "F18";
	case VK_F19:
		return "F19";
	case VK_F20:
		return "F20";
	case VK_F21:
		return "F21";
	case VK_F22:
		return "F22";
	case VK_F23:
		return "F23";
	case VK_F24:
		return "F24";
	case VK_NUMLOCK:
		return "NUM LOCK";
	case VK_SCROLL:
		return "SCROLL LOCK";
	case VK_LSHIFT:
		return "LEFT SHIFT";
	case VK_RSHIFT:
		return "RIGHT SHIFT";
	case VK_LCONTROL:
		return "LEFT CTRL";
	case VK_RCONTROL:
		return "RIGHT CTRL";
	case VK_LMENU:
		return "LEFT ALT";
	case VK_RMENU:
		return "RIGHT ALT";
	case VK_OEM_1:
		return "SEMICOLON/COLON";
	case VK_OEM_PLUS:
		return "PLUS/EQUALS";
	case VK_OEM_COMMA:
		return "COMMA/LESSTHAN";
	case VK_OEM_MINUS:
		return "MINUS/UNDERSCORE";
	case VK_OEM_PERIOD:
		return "PERIOD/GREATERTHAN";
	case VK_OEM_2:
		return "FORWARD SLASH/QUESTION";
	case VK_OEM_3:
		return "TILDE/SQUIGGLE";
	case VK_OEM_4:
		return "LEFTSQUARE/LEFTBRACKET";
	case VK_OEM_5:
		return "BACKSLASH/PIPE";
	case VK_OEM_6:
		return "RIGHTSQUARE/RIGHTBRACKET";
	case VK_OEM_7:
		return "APOSTROPHE/QUOTE";
	case 0x30:
		return "0";
	case 0x31:
		return "1";
	case 0x32:
		return "2";
	case 0x33:
		return "3";
	case 0x34:
		return "4";
	case 0x35:
		return "5";
	case 0x36:
		return "6";
	case 0x37:
		return "7";
	case 0x38:
		return "8";
	case 0x39:
		return "9";
	case 0x41:
		return "A";
	case 0x42:
		return "B";
	case 0x43:
		return "C";
	case 0x44:
		return "D";
	case 0x45:
		return "E";
	case 0x46:
		return "F";
	case 0x47:
		return "G";
	case 0x48:
		return "H";
	case 0x49:
		return "I";
	case 0x4A:
		return "J";
	case 0x4B:
		return "K";
	case 0x4C:
		return "L";
	case 0x4D:
		return "M";
	case 0x4E:
		return "N";
	case 0x4F:
		return "O";
	case 0x50:
		return "P";
	case 0x51:
		return "Q";
	case 0x52:
		return "R";
	case 0x53:
		return "S";
	case 0x54:
		return "T";
	case 0x55:
		return "U";
	case 0x56:
		return "V";
	case 0x57:
		return "W";
	case 0x58:
		return "X";
	case 0x59:
		return "Y";
	case 0x5A:
		return "Z";
	default:
		return "UNKOWN KEYCODE";
	}
}
