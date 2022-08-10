#pragma once
#include "Engine_Defines.h"


#include "Json/json.hpp"
#include <fstream>

using json = nlohmann::json;

#include "Imgui/imgui.h"

#include <filesystem>


namespace Client
{
	static const unsigned int g_iWinCX = 1280;
	static const unsigned int g_iWinCY = 720;
	static const unsigned int g_iTileCX = 2;
	static const unsigned int g_iTileCY = 2;
	static const unsigned int g_iTileX = 150;


	static const unsigned int g_iMinLevelID = 0;
	static const unsigned int g_iMaxLevelID = 99;
	static const unsigned int g_iMinGameObjectID = 100;
	static const unsigned int g_iMaxGameObjectID = 9999;
	static const unsigned int g_iMinComponentID = 10000;
	static const unsigned int g_iMaxComponentID = 99999;

	static unsigned int g_iCurLevelID = 0;
	static unsigned int g_iCurGameObjectID = 100;
	static unsigned int g_iCurComponentID = 10000;
}

#include "Client_Enum.h"
#include "Client_Functor.h"
#include "Client_Macro.h"
#include "Client_Struct.h"


extern HWND			g_hWnd;
extern HINSTANCE	g_hInst;

using namespace Client;
