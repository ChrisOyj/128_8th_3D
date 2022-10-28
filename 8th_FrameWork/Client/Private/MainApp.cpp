#include "stdafx.h"
#include "..\Public\MainApp.h"

#include "GameInstance.h"

#include "Loading_Manager.h"

#include "CLevel_Loading.h"
#include "CUser.h"

#include "CCamera_Free.h"
#include "CCamera_Default.h"

#include "CGameObject_Factory.h"
#include "CPrototype_Factory.h"

#include "ImGui_Manager.h"
#include "CState_Manager.h"
#include "CEffects_Factory.h"
#include "Transform.h"


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

	if (FAILED(CPrototype_Factory::SetUp_DefaultComponents()))
		return E_FAIL;

	if (FAILED(CPrototype_Factory::SetUp_DefaultGameObjects()))
		return E_FAIL;

	if (FAILED(CLoading_Manager::Get_Instance()->Initialize()))
		return E_FAIL;

	if (FAILED(SetUp_Statics()))
		return E_FAIL;

	if (FAILED(CState_Manager::Get_Instance()->Initialize()))
		return E_FAIL;

	if (FAILED(CLoading_Manager::Get_Instance()->Reserve_Load_Level(LEVEL_LOGO)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Load_SoundFile(L"../bin/resources/sounds/ingame")))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Load_SoundFile(L"../bin/resources/sounds/ui")))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Progress()
{
	if (KEY(F9, TAP))
	{
		CLoading_Manager::Get_Instance()->Reserve_Load_Level(LEVEL_UNITY);
	}

	if (FAILED(CLoading_Manager::Get_Instance()->Load_Level()))
	{
		Call_MsgBox(L"Failed to Load_Level : CLoading_Manager");
		return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Tick_Engine()))
	{
		Call_MsgBox(L"Failed to Tick_Engine : CMainApp");
		return E_FAIL;
	}

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

	if (FAILED(m_pGameInstance->Clear_BackBuffer_View(_float4(0.9f, 0.9f, 0.9f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Clear_DepthStencil_View()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Render_Engine()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Present()))
		return E_FAIL;


	return S_OK;
}

void CMainApp::Release()
{
	CEffects_Factory::Get_Instance()->Destroy_Instance();

	CLoading_Manager::Get_Instance()->Destroy_Instance();
	CUser::Get_Instance()->Destroy_Instance();
#ifdef _DEBUG
	CImGui_Manager::Get_Instance()->Destroy_Instance();
#endif
	CState_Manager::Get_Instance()->Destroy_Instance();
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
	SoundDesc.iChannelNumbers[CHANNEL_UI] = 4;
	SoundDesc.iChannelNumbers[CHANNEL_ENVIRONMENT] = 9;
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
	CREATE_STATIC(pDefaultCam, HASHCODE(CCamera_Default));
	CGameInstance::Get_Instance()->Add_Camera(L"Default", pDefaultCam);
	CGameInstance::Get_Instance()->Change_Camera(L"Default");


	/* Free Camera */
	CCamera_Free* pFreeCam = CCamera_Free::Create();
	pFreeCam->Initialize();
	CREATE_STATIC(pFreeCam, HASHCODE(CCamera_Free));
	DISABLE_GAMEOBJECT(pFreeCam);
	CGameInstance::Get_Instance()->Add_Camera(L"Free", pFreeCam);

	

	return S_OK;
}

HRESULT CMainApp::SetUp_ShaderFiles()
{
	if (FAILED(CGameInstance::Get_Instance()->Load_EffectFile(L"../bin/shaderfiles/Shader_VtxTex.hlsl")))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Load_EffectFile(L"../bin/shaderfiles/Shader_VtxCubeTex.hlsl")))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Load_EffectFile(L"../bin/shaderfiles/Shader_VtxnorTex.hlsl")))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Load_EffectFile(L"../bin/shaderfiles/Shader_VtxModel.hlsl")))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Load_EffectFile(L"../bin/shaderfiles/Shader_VtxAnimModel.hlsl")))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Load_EffectFile(L"../bin/shaderfiles/Shader_VtxInstance.hlsl")))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Load_EffectFile(L"../bin/shaderfiles/Shader_VtxRectInstance.hlsl")))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Load_EffectFile(L"../bin/shaderfiles/Shader_VtxTriInstance.hlsl")))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Load_EffectFile(L"../bin/shaderfiles/Shader_VtxModelEffects.hlsl")))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Load_EffectFile(L"../bin/shaderfiles/Shader_VtxDecalCube.hlsl")))
		return E_FAIL;

	return S_OK;
}
