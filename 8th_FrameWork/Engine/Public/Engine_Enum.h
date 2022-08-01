#pragma once

namespace Engine
{

	enum MOUSEMOVE { MMS_X, MMS_Y, MMS_WHEEL, MMS_END };

	enum RENDER_GROUP { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END };

	enum OBJ_GROUP
	{
		GR_DEFAULT,
		GR_END = 32
	};

	enum COL_GROUP
	{
		COL_DEFAULT,
		COL_END = 32
	};

	enum COM_GROUP
	{
		COM_GROUP_DEFAULT,
		COM_GROUP_END = 32
	};

#pragma region CHANNEL_ENUM

	enum CHANNEL_GROUP
	{
		CH_BGM,
		CH_GROUP_0,
		CH_GROUP_1,
		CH_GROUP_2,
		CH_GROUP_3,
		CH_GROUP_END = 5
	};


#pragma endregion




#pragma region KEY_MANGER

	enum class KEY
	{
		LEFT = 0,
		RIGHT,
		UP,
		DOWN,
		NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9, NUM0,
		F1, F2, F3, F4, F5, F6, F7, F8, F9,
		Q, W, E, R, T, Y, U, I, O, P,
		A, S, D, F, G, H, J, K, L,
		Z, X, C, V, B, N, M,
		ALT,
		LSHIFT,
		SPACE,
		CTRL,
		ENTER,
		ESC,
		TAB,
		LBUTTON,
		RBUTTON,
		BACKSPACE,
		DELETEKEY,
		LAST,
	};

	enum class KEY_STATE {
		TAP, // 막 누른 시점
		HOLD,// 꾹 누르고 있음
		AWAY,// 막 뗀 시점
		NONE,// 걍 안눌린 상태
	};
#pragma endregion

}