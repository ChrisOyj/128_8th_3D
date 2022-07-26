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
	/* �ε��� ȭ���̶�, �ε��� ��ü �������� �س��� ������, */
	m_pLoadLevel->Enter();
	/* �̰��� ���� �а�, ��ü �����ϴ� �ڵ� �ۼ� */

	/* �� �����Ǹ� �ε��� ȭ���̶� �ε��� ��ü ������� */


	return S_OK;
}

void CLoad_Manager::Release()
{
	WaitForSingleObject(m_hThread, INFINITE);
	DeleteCriticalSection(&m_CriticalSection);
	CloseHandle(m_hThread);
}
