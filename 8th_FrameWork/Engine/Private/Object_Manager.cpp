#include "Object_Manager.h"
#include "GameObject.h"

CObject_Manager::CObject_Manager()
{
}

CObject_Manager::~CObject_Manager()
{
	Release();
}

CGameObject* CObject_Manager::Get_StaticObj(const _uint& iKeyValue)
{
	auto iter = m_mapStaticObj.find(iKeyValue);
	if (iter == m_mapStaticObj.end())
	{
		Call_MsgBox(L"Failed to find StaticObj : CObject_Manager");
		return nullptr;
	}

	return iter->second;
}

void CObject_Manager::Tick()
{
	for (auto& elem : m_mapStaticObj)
		if (elem.second->Is_Valid())
			elem.second->Tick();

	for (_uint i = 0; i < GR_END; ++i)
	{
		for (auto& pGameObject : m_pGameObjects[i])
			if (pGameObject->Is_Valid())
				pGameObject->Tick();
	}
	
}

void CObject_Manager::Late_Tick()
{
	for (auto iter = m_mapStaticObj.begin(); iter != m_mapStaticObj.end();)
	{
		//객체가 dead 상태면 리스트에서 빼주기
		CGameObject* pGameObject = iter->second;

		if (pGameObject->Is_Dead())
			iter = m_mapStaticObj.erase(iter);
		else
		{
			if (!pGameObject->Is_Disable())
				pGameObject->Late_Tick();

			++iter;
		}
	}

	for (_uint i = 0; i < GR_END; ++i)
	{
		for (auto iter = m_pGameObjects[i].begin(); iter != m_pGameObjects[i].end();)
		{
			//객체가 dead 상태면 리스트에서 빼주기
			CGameObject* pGameObject = *iter;

			if (pGameObject->Is_Dead())
				iter = m_pGameObjects[i].erase(iter);
			else
			{
				if (!pGameObject->Is_Disable())
					pGameObject->Late_Tick();

				++iter;
			}
		}
	}
}



void CObject_Manager::Delete_AllObjects()
{
	for (auto& elem : m_mapStaticObj)
		SAFE_DELETE(elem.second);

	m_mapStaticObj.clear();

	for (_uint i = 0; i < GR_END; ++i)
	{
		for (auto& elem : m_pGameObjects[i])
			SAFE_DELETE(elem);

		m_pGameObjects[i].clear();
	}
}

void CObject_Manager::Delete_Objects(const _uint& iGroupIdx)
{
	for (auto& elem : m_pGameObjects[iGroupIdx])
		SAFE_DELETE(elem);

	m_pGameObjects[iGroupIdx].clear();
}

void CObject_Manager::Release()
{
	Delete_AllObjects();
}