#include "..\Public\Load_Manager.h"

#include "Level.h"
#include "Object_Manager.h"

CLoad_Manager::CLoad_Manager()
{
}

CLoad_Manager::~CLoad_Manager()
{
	Release();
}

unsigned int APIENTRY LoadingMain(void* pArg)
{
	CLoad_Manager*		pLoadingMgr = (CLoad_Manager*)pArg;
	CRITICAL_SECTION	CS = pLoadingMgr->Get_CS();
	EnterCriticalSection(&CS);

	HRESULT hr = 0;
	if (FAILED(pLoadingMgr->Start_Loading()))
		Call_MsgBox(L"Failed to Start_Loading : CLoad_Manager");

	LeaveCriticalSection(&CS);

	return 0;
}


HRESULT CLoad_Manager::Load_Level(CLevel * pLevel)
{
	m_pLoadLevel = pLevel;
	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoad_Manager::Start_Loading()
{
	/* 로딩중 화면이랑, 로딩바 객체 생성부터 해놔서 돌리고, */
	m_pLoadLevel->Enter();
	/* 이곳에 파일 읽고, 객체 생성하는 코드 작성 */

	/* 다 생성되면 로딩중 화면이랑 로딩바 객체 지우야함 */


	return S_OK;
}

void CLoad_Manager::Release()
{
	WaitForSingleObject(m_hThread, INFINITE);
	DeleteCriticalSection(&m_CriticalSection);
	CloseHandle(m_hThread);
}
