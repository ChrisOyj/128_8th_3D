#pragma once
#include "Client_Defines.h"


BEGIN(Client)

class CImGui_Window abstract
{
	friend class CImGui_Manager;

	struct ImGuiDESC
	{
		string				strName;
		ImGuiWindowFlags	eWindowFlags = ImGuiWindowFlags_None;
		ImVec2				vWindowSize = { 0.f, 0.f };

	};

protected:
	CImGui_Window();
	virtual ~CImGui_Window();

public:
	virtual HRESULT	Initialize() PURE;
	virtual void	Tick() PURE;
	virtual HRESULT	Render() PURE;

protected:
	ImGuiDESC		m_tImGuiDESC;

protected:
	HRESULT			SetUp_ImGuiDESC(string _strName, const ImVec2& _vWindowSize, ImGuiWindowFlags _eWindowFlags);
	HRESULT			Begin();
	HRESULT			End();


};

END

