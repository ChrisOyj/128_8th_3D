#include "stdafx.h"
#include "Loading_Manager.h"

#include "CLevel_Loading.h"
#include "CLevel_Unity.h"
#include "CLevel_Logo.h"
#include "CLevel_MainMenu.h"
#include "GameInstance.h"

#include "CShader.h"

IMPLEMENT_SINGLETON(CLoading_Manager)

CLoading_Manager::CLoading_Manager()
{
	ZeroMemory(m_arrLevels, sizeof(CLevel) * LEVEL_END);
}

CLoading_Manager::~CLoading_Manager()
{
	Release();
}

unsigned int APIENTRY LoadingMain(void* pArg)
{
	CoInitializeEx(nullptr, 0);

	CLoading_Manager*		pLoadingMgr = (CLoading_Manager*)pArg;
	CRITICAL_SECTION	CS = pLoadingMgr->Get_CS();
	EnterCriticalSection(&CS);

	HRESULT hr = 0;
	if (FAILED(pLoadingMgr->Start_Loading()))
		Call_MsgBox(L"Failed to Start_Loading : CLoad_Manager");

	LeaveCriticalSection(&CS);

	return 0;
}


HRESULT CLoading_Manager::Initialize()
{
	for (_uint i = LEVEL_LOADING; i < LEVEL_END; ++i)
	{
		m_arrLevels[i] = nullptr;
	}

	m_arrLevels[LEVEL_LOADING] = CLevel_Loading::Create();
	m_arrLevels[LEVEL_UNITY] = CLevel_Unity::Create();
	m_arrLevels[LEVEL_LOGO] = CLevel_Logo::Create();
	m_arrLevels[LEVEL_MAINMENU] = CLevel_MainMenu::Create();

	


	return S_OK;
}

HRESULT CLoading_Manager::Reserve_Load_Level(LEVEL_TYPE_CLIENT eLevelID)
{
	if (eLevelID < 0 || eLevelID >= LEVEL_END)
	{
		Call_MsgBox_Index(L"Out of Range in Load_Level : CLoading_Manager", eLevelID);
		return E_FAIL;
	}

	m_eLoadID = eLevelID;
	m_bReservation = true;

	return S_OK;
}

HRESULT CLoading_Manager::Load_Level()
{
	if (!m_bReservation)
		return S_OK;

	m_bReservation = false;

	/* Delete all Objects except Statics */
	for (_uint i = 0; i < GR_END; ++i)
	{
		CGameInstance::Get_Instance()->Delete_Objects(i);
	}

	CGameInstance::Get_Instance()->Clear_All_Components();
	//CGameInstance::Get_Instance()->Delete_GameObject_Prototypes();

	m_bFinish = false;
	//static_cast<CLevel_Loading*>(m_arrLevels[LEVEL_LOADING])->Set_NextLevel(eLevelID);
	CHANGE_LEVEL(m_arrLevels[LEVEL_LOADING]);


	InitializeCriticalSection(&m_CriticalSection);
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoading_Manager::Start_Loading()
{
	/* 로딩중 화면이랑, 로딩바 객체 생성부터 해놔서 돌리고, */


	/* 이곳에 파일 읽고, 객체 생성하는 코드 작성 */
	m_arrLevels[m_eLoadID]->SetUp_Prototypes();

	/* 다 생성되면 로딩중 화면이랑 로딩바 객체 지우야함 */

	Finish_LoadingThread();
	return S_OK;
}

void CLoading_Manager::Open_NextLevel()
{
	CHANGE_LEVEL(m_arrLevels[m_eLoadID]);
}

void CLoading_Manager::Release()
{
	while (m_hThread == 0)
	{
		continue;
	}

	for (_uint i = 0; i < LEVEL_END; ++i)
	{
		SAFE_DESTROY(m_arrLevels[i]);
	}

	
}

void CLoading_Manager::Set_ShaderResource(CShader* pShader, const char* pConstantName)
{
	_float fResource = 1.f;

	fResource = m_arrLevels[m_eLoadID]->Get_LoadingFinish();

	pShader->Set_RawValue(pConstantName, &fResource, sizeof(_float));


	/*pShader->CallBack_SetRawValues +=
		bind(&CLevel_Loading::Set_ShaderResource, this, placeholders::_1, variablename);*/
}

void CLoading_Manager::Finish_LoadingThread()
{
	m_bFinish = true;

	WaitForSingleObject(m_hThread, INFINITE);
	DeleteCriticalSection(&m_CriticalSection);
	CloseHandle(m_hThread);
	m_hThread = 0;
}
