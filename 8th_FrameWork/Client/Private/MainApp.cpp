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
#include "CPrototype_Factory.h"

#include "ImGui_Manager.h"

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

	if (FAILED(CLoading_Manager::Get_Instance()->Initialize()))
		return E_FAIL;

	if (FAILED(CUser::Get_Instance()->Initialize()))
		return E_FAIL;

	if (FAILED(SetUp_Statics()))
		return E_FAIL;

	if (FAILED(CLoading_Manager::Get_Instance()->Reserve_Load_Level(LEVEL_UNITY)))
		return E_FAIL;

	if (FAILED(SetUp_CurID()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Progress()
{
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

	if (FAILED(m_pGameInstance->Clear_BackBuffer_View(_float4(0.f, 0.f, 0.f, 1.f))))
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

HRESULT CMainApp::SetUp_CurID()
{
	g_iCurLevelID = g_iMinLevelID;
	g_iCurUIID = g_iMinUIID;
	g_iCurGameObjectID = g_iMinGameObjectID;
	g_iCurComponentID = g_iMinComponentID;

	for (filesystem::directory_iterator FileIter("../bin/Json");
		FileIter != filesystem::end(FileIter); ++FileIter)
	{
		const filesystem::directory_entry& entry = *FileIter;

		wstring wstrPath = entry.path().relative_path();
		string strFullPath;
		strFullPath.assign(wstrPath.begin(), wstrPath.end());

		_int iFind = (_int)strFullPath.rfind("\\") + 1;
		string strFileName = strFullPath.substr(iFind, strFullPath.length() - iFind);

		_int iFind2 = (_int)strFileName.find(".");
		strFileName = strFileName.substr(0, iFind2);

		_uint iFileNum = stoi(strFileName);
		_uint iNewID = iFileNum + 1;

		if (Safe_CheckID(iFileNum, ID_COMPONENT))
		{
			g_iCurComponentID = max(g_iCurComponentID, iNewID);

		}
		else if (Safe_CheckID(iFileNum, ID_MESHGAMEOBJECT))
		{
			g_iCurGameObjectID = max(g_iCurGameObjectID, iNewID);
		}
		else if (Safe_CheckID(iFileNum, ID_UI))
		{
			g_iCurUIID = max(g_iCurUIID, iNewID);
		}
		else
		{
			g_iCurLevelID = max(g_iCurLevelID, iNewID);
		}
	}
	return S_OK;
}
