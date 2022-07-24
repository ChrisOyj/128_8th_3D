#include "Object_Manager.h"
#include "GameObject.h"

CObject_Manager::CObject_Manager()
{
}

CObject_Manager::~CObject_Manager()
{
	Release();
}

void CObject_Manager::Tick()
{
	for (auto& elem : m_mapStaticObj)
		if (elem.second->IsValid())
			elem.second->Tick();

	for (auto& elem : m_pGameObjects)
		if (elem->IsValid())
			elem->Tick();
}

void CObject_Manager::Late_Tick()
{
	for (auto iter = m_mapStaticObj.begin(); iter != m_mapStaticObj.end();)
	{
		//객체가 dead 상태면 리스트에서 빼주기
		CGameObject* pGameObject = iter->second;

		if (pGameObject->IsDead())
			iter = m_mapStaticObj.erase(iter);
		else
		{
			if (!pGameObject->IsDisable())
				pGameObject->Late_Tick();

			++iter;
		}
	}


	for (auto iter = m_pGameObjects.begin(); iter != m_pGameObjects.end();)
	{
		//객체가 dead 상태면 리스트에서 빼주기
		CGameObject* pGameObject = *iter;

		if (pGameObject->IsDead())
			iter = m_pGameObjects.erase(iter);
		else
		{
			if (!pGameObject->IsDisable())
				pGameObject->Late_Tick();

			++iter;
		}
	}
}



void CObject_Manager::Delete_All()
{
	for (auto& elem : m_mapStaticObj)
		elem.second->Destory_Instance();

	m_mapStaticObj.clear();

	for (auto& elem : m_pGameObjects)
		elem->Destory_Instance();
		//delete elem;

	m_pGameObjects.clear();
}

void CObject_Manager::Delete_Objects()
{
	for (auto& elem : m_pGameObjects)
		elem->Destory_Instance();

	m_pGameObjects.clear();
}

void CObject_Manager::Release()
{
	Delete_All();
}