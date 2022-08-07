#include "stdafx.h"
#include "..\Public\MainApp.h"

#include "GameInstance.h"

#include "Loading_Manager.h"

#include "CLevel_Default.h"
#include "CLevel_Loading.h"
#include "CUser.h"

#include "CCamera_Free.h"
#include "CCamera_Default.h"

#include "CGameObject_Factory.h"

#include "ImGui_Manager.h"

IMPLEMENT_SINGLETON(CMainApp);

CMainApp::CMainApp()
{
}

CMainApp::~CMainApp()
{
	Release();
}

HRESULT CMainApp::Initialize()
{
	m_pGameInstance = CGameInstance::Get_Instance();

	if (nullptr == m_pGameInstance)
		return E_FAIL;


	if (FAILED(SetUp_Engine()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderFiles()))
		return E_FAIL;

	if (FAILED(CLoading_Manager::Get_Instance()->Initialize()))
		return E_FAIL;

	if (FAILED(CUser::Get_Instance()->Initialize()))
		return E_FAIL;

	if (FAILED(SetUp_Statics()))
		return E_FAIL;

	if (FAILED(CImGui_Manager::Get_Instance()->Initialize()))
		return E_FAIL;

	CLoading_Manager::Get_Instance()->Load_Level(LEVEL_UNITY);

	return S_OK;
}

HRESULT CMainApp::Progress()
{
	if (FAILED(m_pGameInstance->Tick_Engine()))
	{
		Call_MsgBox(L"Failed to Tick_Engine : CMainApp");
		return E_FAIL;
	}

	CImGui_Manager::Get_Instance()->Tick();

	if (FAILED(Render()))
	{
		Call_MsgBox(L"Failed to Render : CMainApp");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMainApp::Render()
{
	if (!m_pGameInstance->Can_Update())
		return S_OK;

	if (FAILED(m_pGameInstance->Clear_BackBuffer_View(_float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Clear_DepthStencil_View()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Render_Engine()))
		return E_FAIL;

	if (FAILED(CImGui_Manager::Get_Instance()->Render()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Present()))
		return E_FAIL;


	return S_OK;
}

void CMainApp::Release()
{
	CLoading_Manager::Get_Instance()->Destroy_Instance();
	CUser::Get_Instance()->Destroy_Instance();
	CImGui_Manager::Get_Instance()->Destroy_Instance();
	SAFE_DESTROY(m_pGameInstance);

}

HRESULT CMainApp::SetUp_Engine()
{
	GRAPHICDESC		GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof(GRAPHICDESC));

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.hInst = g_hInst;
	GraphicDesc.iWinCX = g_iWinCX;
	GraphicDesc.iWinCY = g_iWinCY;
	GraphicDesc.WinMode = GRAPHICDESC::MODE_WIN;

	SOUNDDESC	SoundDesc;
	SoundDesc.iChannelNumbers[CHANNEL_BGM] = 1;
	SoundDesc.iChannelNumbers[CHANNEL_PLAYER] = 5;
	SoundDesc.iChannelNumbers[CHANNEL_ENVIRONMENT] = 8;
	SoundDesc.iChannelNumbers[CHANNEL_EFFECTS] = 13;
	SoundDesc.iChannelNumbers[CHANNEL_VOICE] = 5;

	if (FAILED(m_pGameInstance->Initialize_Engine(GraphicDesc, SoundDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::SetUp_Levels()
{
	return S_OK;
}

HRESULT CMainApp::SetUp_Statics()
{
	/*Default Cam*/
	CCamera* pDefaultCam = CCamera_Default::Create();
	pDefaultCam->Initialize();
	CREATE_STATIC(pDefaultCam, 111113);
	CGameInstance::Get_Instance()->Add_Camera(L"Default", pDefaultCam);
	CGameInstance::Get_Instance()->Change_Camera(L"Default");


	/* Free Camera */
	CCamera_Free* pFreeCam = CCamera_Free::Create();
	pFreeCam->Initialize();
	CREATE_STATIC(pFreeCam, 111114);
	DISABLE_GAMEOBJECT(pFreeCam);
	CGameInstance::Get_Instance()->Add_Camera(L"Free", pFreeCam);

	

	return S_OK;
}

HRESULT CMainApp::SetUp_ShaderFiles()
{
	if (FAILED(CGameInstance::Get_Instance()->Load_EffectFile(L"../bin/shaderfiles/Shader_VtxTex.hlsl")))
		return E_FAIL;

	return S_OK;
}
