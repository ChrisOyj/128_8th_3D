#include "stdafx.h"
#include "..\Public\MainApp.h"

#include "GameInstance.h"
#include "CLevel_Default.h"

CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::Get_Instance())
{
}

CMainApp::~CMainApp()
{
	Release();
}

HRESULT CMainApp::Initialize()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	if (
		FAILED(SetUp_Engine()) ||
		FAILED(SetUp_Levels())
		)
		return E_FAIL;


	return S_OK;
}

HRESULT CMainApp::Progress()
{
	m_pGameInstance->Tick_Engine();

	//============
	//Render
	//============
	if (FAILED(Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Render()
{
	if (FAILED(m_pGameInstance->Render_Engine()))
		return E_FAIL;

	return S_OK;
}

void CMainApp::Release()
{
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
	CGameInstance::Get_Instance()->Reserve_Level(LEVEL_END);

	CLevel* pLevel = CLevel_Default::Create(L"");
	CGameInstance::Get_Instance()->Add_Level(pLevel);




	if (false == CGameInstance::Get_Instance()->Is_AllLevelsReady())
		return E_FAIL;


	m_pGameInstance->Change_Level(LEVEL_LOGO);

	return S_OK;
}
