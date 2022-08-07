#pragma once
#include "Client_Defines.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

class CImGui_Window;

class CImGui_Manager
{
	enum IMGUI_WINDOW_TYPE
	{
		IMGUI_DEFAULT,
		IMGUI_END
	};

	DECLARE_SINGLETON(CImGui_Manager)

private:
	CImGui_Manager();
	~CImGui_Manager();

public:
	HRESULT Initialize();
	void	Tick();
	HRESULT Render();

private:
	CImGui_Window* m_arrWindows[IMGUI_END];

private:
	void Release();
};

END