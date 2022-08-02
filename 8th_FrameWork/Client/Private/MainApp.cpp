#include "stdafx.h"
#include "..\Public\MainApp.h"

#include "GameInstance.h"
#include "CLevel_Default.h"

#include "Loading_Manager.h"

#include "CCamera_Free.h"

#include "CGameObject_Factory.h"

IMPLEMENT_SINGLETON(CMainApp)

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

	if (FAILED(CLoading_Manager::Get_Instance()->Initialize()))
		return E_FAIL;

	if (FAILED(SetUp_Statics()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Progress()
{
	if (FAILED(m_pGameInstance->Tick_Engine()))
		return E_FAIL;

	if (FAILED(Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Render()
{
	m_pGameInstance->Clear_BackBuffer_View(_float4(0.f, 0.f, 1.f, 1.f));
	m_pGameInstance->Clear_DepthStencil_View();

	if (FAILED(m_pGameInstance->Render_Engine()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Present()))
		return E_FAIL;


	return S_OK;
}

void CMainApp::Release()
{
	m_pGameInstance->Destroy_Instance();
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
	/* Free Camera */
	CCamera_Free* pFreeCam = CCamera_Free::Create();
	CGameInstance::Get_Instance()->Add_GameObject_Prototype(pFreeCam->Get_ID(), pFreeCam);

	CGameObject* pFreeCamClone = CGameObject_Factory::Create_FromPrototype(pFreeCam->Get_ID());

	CREATE_STATIC(pFreeCamClone, pFreeCamClone->Get_ID());
	CGameInstance::Get_Instance()->Add_Camera(L"Free", (CCamera_Free*)pFreeCam);
	CGameInstance::Get_Instance()->Change_Camera(L"Free");

	return S_OK;
}
