#include "stdafx.h"
#include "Loading_Manager.h"

#include "CLevel_Default.h"
#include "CLevel_Unity.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CLoading_Manager)

CLoading_Manager::CLoading_Manager()
{
	ZeroMemory(m_arrLevels, sizeof(CLevel) * LEVEL_END);
}

CLoading_Manager::~CLoading_Manager()
{
	Release();
	DeleteCriticalSection(&m_CriticalSection);
	CloseHandle(m_hThread);
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
	m_arrLevels[LEVEL_UNITY] = CLevel_Unity::Create();

	/*for (_uint i = LEVEL_LOGO; i < LEVEL_END; ++i)
	{
		m_arrLevels[i] = CLevel_Default::Create((LEVEL_TYPE_CLIENT)i);

		if (!m_arrLevels[i])
		{
			Call_MsgBox_Index(L"Failed to Create Level : CLoading_Manager", i);
			return E_FAIL;
		}
	}*/

	Load_Level(LEVEL_UNITY);

	return S_OK;
}

HRESULT CLoading_Manager::Load_Level(LEVEL_TYPE_CLIENT	eLevelID)
{
	/* Delete all Objects except Statics */

	for (_uint i = 0; i < GR_END; ++i)
	{
		CGameInstance::Get_Instance()->Delete_Objects(i);
	}

	m_eLoadID = eLevelID;
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
	m_arrLevels[m_eLoadID]->Enter();

	/* 다 생성되면 로딩중 화면이랑 로딩바 객체 지우야함 */

	CHANGE_LEVEL(m_arrLevels[m_eLoadID]);
	Finish_LoadingThread();
	return S_OK;
}

void CLoading_Manager::Release()
{
	for (_uint i = 0; i < LEVEL_LOGO; ++i)
	{
		m_arrLevels[i]->Destroy_Instance();
		m_arrLevels[i] = nullptr;
	}
	
	//Finish_LoadingThread();
}

void CLoading_Manager::Finish_LoadingThread()
{
	WaitForSingleObject(m_hThread, INFINITE);
	DeleteCriticalSection(&m_CriticalSection);
	CloseHandle(m_hThread);
}
