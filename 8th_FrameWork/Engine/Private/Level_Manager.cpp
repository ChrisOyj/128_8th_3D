#include "..\Public\Level_Manager.h"
#include "Level.h"
#include "GameInstance.h"
#include "Load_Manager.h"
#include "Prototype_Manager.h"
#include "Object_Manager.h"

CLevel_Manager::CLevel_Manager()
{
}

CLevel_Manager::~CLevel_Manager()
{
	Release();
}

void CLevel_Manager::Tick()
{
	m_pCurrentLevel->Tick();
}

void CLevel_Manager::Late_Tick()
{
	m_pCurrentLevel->Late_Tick();
}

HRESULT CLevel_Manager::Render()
{
	if (FAILED(m_pCurrentLevel->Render()))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_Manager::Open_Level(_uint iLevelID, CLevel * pLevel)
{
	/* 이전레벨에서 사용한 자원들을 모두 소거한다. */
	if (m_pCurrentLevel)
	{
		m_pCurrentLevel->Exit();
		m_pCurrentLevel->Destory_Instance();
		CObject_Manager::Get_Instance()->Delete_Objects();
		CPrototype_Manager::Get_Instance()->Clear();
	}

	m_pCurrentLevel = pLevel;
	m_iCurrentLevelID = iLevelID;

	m_pCurrentLevel->Enter();

	//CLoad_Manager::Get_Instance()->Load_Level(m_pCurrentLevel);

	return S_OK;
}

void CLevel_Manager::Release()
{
	if (m_pCurrentLevel)
		m_pCurrentLevel->Destory_Instance();
}
