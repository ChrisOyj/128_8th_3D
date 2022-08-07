#include "stdafx.h"
#include "CImGui_Window.h"

CImGui_Window::CImGui_Window()
{
}

CImGui_Window::~CImGui_Window()
{
}

HRESULT CImGui_Window::SetUp_ImGuiDESC(string _strName, const ImVec2& _vWindowSize, ImGuiWindowFlags _eWindowFlags)
{
	m_tImGuiDESC.strName = _strName;
	m_tImGuiDESC.vWindowSize = _vWindowSize;
	m_tImGuiDESC.eWindowFlags = _eWindowFlags;

	return S_OK;
}

HRESULT CImGui_Window::Begin()
{
	ImGui::SetNextWindowSize(m_tImGuiDESC.vWindowSize, 0);

	if (!ImGui::Begin(m_tImGuiDESC.strName.c_str(), 0, m_tImGuiDESC.eWindowFlags))
		return E_FAIL;

	return S_OK;
}

HRESULT CImGui_Window::End()
{
	ImGui::End();

	return S_OK;
}
