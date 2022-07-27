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

HRESULT CLevel_Manager::Reserve_Level(const _uint& iLevelSize)
{
	m_arrLevels.reserve(iLevelSize);
	
	for (_uint i = 0; i < iLevelSize; ++i)
		m_arrLevels.push_back(nullptr);

	return S_OK;
}

HRESULT CLevel_Manager::Add_Level(CLevel* pLevel)
{
	m_arrLevels.push_back(pLevel);

	return S_OK;
}

_bool CLevel_Manager::Is_AllLevelsReady()
{
	if (m_arrLevels.back() == nullptr)
		return false;

	return true;
}

void CLevel_Manager::Tick()
{
	m_arrLevels[m_iCurrentLevelID]->Tick();
}

void CLevel_Manager::Late_Tick()
{
	m_arrLevels[m_iCurrentLevelID]->Late_Tick();
}

HRESULT CLevel_Manager::Render()
{
	if (FAILED(m_arrLevels[m_iCurrentLevelID]->Render()))
	{
		Call_MsgBox(L"Failed to Render : CLevel_Manager");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CLevel_Manager::Open_Level(const _uint& iLevelID)
{
	/* 이전레벨에서 사용한 자원들을 모두 소거한다. */
	if (m_iCurrentLevelID != 0)
	{
		m_arrLevels[m_iCurrentLevelID]->Exit();
		CObject_Manager::Get_Instance()->Delete_All();
		CPrototype_Manager::Get_Instance()->Clear();
	}

	m_iCurrentLevelID = iLevelID;
	CLoad_Manager::Get_Instance()->Load_Level(m_arrLevels[m_iCurrentLevelID]);

	return S_OK;
}

void CLevel_Manager::Release()
{
	for (size_t i = 0; i < m_arrLevels.size(); ++i)
		delete m_arrLevels[i];

	m_arrLevels.clear();
}
