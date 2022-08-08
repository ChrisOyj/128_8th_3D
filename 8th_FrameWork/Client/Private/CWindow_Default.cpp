#include "stdafx.h"
#include "CWindow_Default.h"

CWindow_Default::CWindow_Default()
{
}

CWindow_Default::~CWindow_Default()
{
}

CWindow_Default* CWindow_Default::Create()
{
	CWindow_Default* pInstance = new CWindow_Default;

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CWindow_Default");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CWindow_Default::Initialize()
{
	m_bEnable = false;
	SetUp_ImGuiDESC(typeid(CWindow_Default).name(), ImVec2(400.f, 50.f), ImGuiWindowFlags_AlwaysAutoResize);

	return S_OK;
}

void CWindow_Default::Tick()
{
}

HRESULT CWindow_Default::Render()
{
	if (FAILED(__super::Begin()))
		return E_FAIL;

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	__super::End();

	return S_OK;
}
