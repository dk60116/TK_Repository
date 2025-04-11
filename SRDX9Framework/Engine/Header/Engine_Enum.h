#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum WINMODE { MODE_FULL, MODE_WIN };

	// Dynamic 컴포넌트 경우 매 프레임마다 갱신해야하는 컴포넌트 집단
	enum COMPONENTID { ID_DYNAMIC, ID_STATIC, ID_END };

	enum INFO {	INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };
	enum ROTATION { ROT_X, ROT_Y, ROT_Z, ROT_END };

	enum TEXTUREID { TEX_NORMAL, TEX_CUBE, TEX_END };

	enum RENDERID { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END };

	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };
	enum MOUSEMOVESTATE {	DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

	enum Layer {DEFAULT, UI, LAYER_END};

	enum KEY_CODE
	{
		KEY_START = 0,

		SERO = 0x30, ONE = 0x31, TWO = 0x32, THREE = 0x33, FOUR = 0x34, FIVE = 0x35, SIX = 0x36, SEVEN = 0x37, EIGHT = 0x38, NINE = 0x39,

		Q = 'Q', W = 'W', E = 'E', R = 'R', T = 'T', Y = 'Y', U = 'U', I = 'I', O = 'O', P = 'P',
		A = 'A', S = 'S', D = 'D', F = 'F', G = 'G', H = 'H', J = 'J', K = 'K', L = 'L',
		Z = 'Z', X = 'X', C = 'C', V = 'V', B = 'B', N = 'N', M = 'M',

		SHIFT = VK_SHIFT, L_SHIFT = VK_LSHIFT, R_SHIFT = VK_RSHIFT, CONTROL = VK_CONTROL, L_CONTROL = VK_LCONTROL, R_CONTROL = VK_RCONTROL,
		ALT = VK_MENU, L_ALT = VK_LMENU, R_ALT = VK_RMENU,

		RETURN = VK_RETURN, ESCAPE = VK_ESCAPE, SPACE = VK_SPACE, TAB = VK_TAB,

		UP = VK_UP, DOWN = VK_DOWN, LEFT = VK_LEFT, RIGHT = VK_RIGHT,

		MOUSE_L = VK_LBUTTON, MOUSE_R = VK_RBUTTON, MOUSEWHILL = WM_MOUSEWHEEL,

		KEY_END
	};

}
#endif // Engine_Enum_h__
