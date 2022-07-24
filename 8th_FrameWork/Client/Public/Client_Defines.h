#pragma once

#include "Engine_Defines.h"
#include "Client_Enum.h"

extern HWND			g_hWnd;
extern HINSTANCE	g_hInst;

namespace Client
{
	static _float3	Get_MousePos(void)
	{
		POINT pt{};

		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		return _float3(float(pt.x), float(pt.y), 0.f);
	}

	static const unsigned int g_iWinCX = 1280;
	static const unsigned int g_iWinCY = 720;
	static const unsigned int g_iTileCX = 2;
	static const unsigned int g_iTileCY = 2;
	static const unsigned int g_iTileX = 150;
	static const unsigned int g_iTileY = 150;

#define COLOR_RED		_float4(0.8f, 0.2f, 0.2f, 1.0f)
#define COLOR_YELLOW	_float4(1.f, 1.f, 0.6f, 1.0f)
#define COLOR_ORANGE	_float4(0.8f, 0.4f, 0.2f, 1.0f)
#define COLOR_GRAY		_float4(0.5f, 0.5f, 0.5f, 1.0f)
#define COLOR_WHITE		_float4(0.9f, 0.9f, 0.9f, 1.0f)
#define COLOR_BLACK		_float4(0.1f, 0.1f, 0.1f, 1.0f)
#define COLOR_BROWN		_float4(0.4f, 0.2f, 0.1f, 1.0f)

#include "Client_Functor.h"

	using namespace Client;
}