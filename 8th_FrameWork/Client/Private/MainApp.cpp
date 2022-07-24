#include "stdafx.h"
#include "..\Public\MainApp.h"
#include "GameInstance.h"
#include "Level_Logo.h"
#include "User.h"
#include "DayNight.h"


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
	GRAPHICDESC		GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof(GRAPHICDESC));

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.hInst = g_hInst;
	GraphicDesc.iWinCX = g_iWinCX;
	GraphicDesc.iWinCY = g_iWinCY;
	GraphicDesc.isWindowMode = true;

	if (FAILED(m_pGameInstance->Initialize_Engine(GraphicDesc)))
		return E_FAIL;

	m_pGameInstance->Change_Level(LEVEL_LOGO, CLevel_Logo::Create());

	::ShowCursor(false);

	return S_OK;
}

HRESULT CMainApp::Progress()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	CUser::Get_Instance()->Tick();
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
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	m_pGameInstance->Render_Begin();
	

	if (FAILED(m_pGameInstance->Render_Engine()))
		return E_FAIL;

	m_pGameInstance->Render_End();

	return S_OK;
}

void CMainApp::Release()
{
}