#pragma once
#include "Engine_Defines.h"



namespace Client
{
	static const unsigned int g_iWinCX = 1280;
	static const unsigned int g_iWinCY = 720;
	static const unsigned int g_iTileCX = 2;
	static const unsigned int g_iTileCY = 2;
	static const unsigned int g_iTileX = 150;
	static const unsigned int g_iTileY = 150;
}

#include "Client_Enum.h"
#include "Client_Functor.h"
#include "Client_Macro.h"
#include "Client_Struct.h"


extern HWND			g_hWnd;
extern HINSTANCE	g_hInst;

using namespace Client;
