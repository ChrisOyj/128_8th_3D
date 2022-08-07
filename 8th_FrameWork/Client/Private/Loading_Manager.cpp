#include "stdafx.h"
#include "Loading_Manager.h"

#include "CLevel_Loading.h"
#include "CLevel_Default.h"
#include "CLevel_Unity.h"
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

	m_arrLevels[LEVEL_LOADING] = CLevel_Loading::Create();
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


	return S_OK;
}

HRESULT CLoading_Manager::Load_Level(LEVEL_TYPE_CLIENT	eLevelID)
{
	if (eLevelID < 0 || eLevelID > LEVEL_END)
	{
		Call_MsgBox_Index(L"Out of Range in Load_Level : CLoading_Manager", eLevelID);
		return E_FAIL;
	}
	/* Delete all Objects except Statics */

	for (_uint i = 0; i < GR_END; ++i)
	{
		CGameInstance::Get_Instance()->Delete_Objects(i);
	}

	CGameInstance::Get_Instance()->Clear_All_Components();
	//CGameInstance::Get_Instance()->Delete_GameObject_Prototypes();

	m_bFinish = false;
	m_eLoadID = eLevelID;
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
	m_arrLevels[m_eLoadID]->Initialize();

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

	for (_uint i = 0; i < LEVEL_LOGO; ++i)
	{
		m_arrLevels[i]->Destroy_Instance();
		m_arrLevels[i] = nullptr;
	}

	
}

void CLoading_Manager::Set_ShaderResource(CShader* pShader, const char* pConstantName)
{
	_float fResource = 1.f;
	CLevel_Default* pLevelDefault = dynamic_cast<CLevel_Default*>(m_arrLevels[m_eLoadID]);

	if (pLevelDefault != nullptr)
	{
		fResource = pLevelDefault->Get_LoadingFinish();
	}

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
