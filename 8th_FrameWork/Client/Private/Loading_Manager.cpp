#include "stdafx.h"
#include "Loading_Manager.h"

#include "CLevel_Default.h"

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
	for (_uint i = 0; i < LEVEL_END; ++i)
	{
		m_arrLevels[i] = CLevel_Default::Create((LEVEL_TYPE_CLIENT)i);

		if (!m_arrLevels[i])
		{
			Call_MsgBox_Index(L"Failed to Create Level : CLoading_Manager", i);
			return E_FAIL;

		}
	}

	return S_OK;
}

HRESULT CLoading_Manager::Load_Level(LEVEL_TYPE_CLIENT	eLevelID)
{
	m_eLoadID = eLevelID;
	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoading_Manager::Start_Loading()
{
	/* �ε��� ȭ���̶�, �ε��� ��ü �������� �س��� ������, */


	/* �̰��� ���� �а�, ��ü �����ϴ� �ڵ� �ۼ� */
	m_arrLevels[m_eLoadID]->Enter();

	/* �� �����Ǹ� �ε��� ȭ���̶� �ε��� ��ü ������� */


	return S_OK;
}

void CLoading_Manager::Release()
{
	WaitForSingleObject(m_hThread, INFINITE);
	DeleteCriticalSection(&m_CriticalSection);
	CloseHandle(m_hThread);
}
