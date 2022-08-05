#include "Object_Manager.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(CObject_Manager)


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

HRESULT CObject_Manager::Regist_GameObject(CGameObject* pComponent, const _uint& iGroupIndex)
{
	if (iGroupIndex < 0 || iGroupIndex >= GR_END)
		return E_FAIL;

	m_pGameObjects[iGroupIndex].push_back(pComponent);

	return S_OK;
}

void CObject_Manager::Check_Objects_Dead()
{
	for (auto iter = m_mapStaticObj.begin(); iter != m_mapStaticObj.end();)
	{
		//객체가 dead 상태면 리스트에서 빼주기
		CGameObject* pGameObject = iter->second;

		if (pGameObject->Is_Dead())
			iter = m_mapStaticObj.erase(iter);
		else
			++iter;
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
				++iter;


		}
	}
	
}

void CObject_Manager::Delete_AllObjects()
{
	for (auto& elem : m_mapStaticObj)
		SAFE_DESTROY(elem.second);

	m_mapStaticObj.clear();

	for (_uint i = 0; i < GR_END; ++i)
	{
		for (auto& elem : m_pGameObjects[i])
			SAFE_DESTROY(elem);

		m_pGameObjects[i].clear();
	}
}

void CObject_Manager::Delete_Objects(const _uint& iGroupIdx)
{
	for (auto& elem : m_pGameObjects[iGroupIdx])
		SAFE_DESTROY(elem);

	m_pGameObjects[iGroupIdx].clear();
}

void CObject_Manager::Add_Object(CGameObject* pGameObject, const _uint& iGroupIdx)
{
	Regist_GameObject(pGameObject, iGroupIdx);
	pGameObject->Set_GroupIndex(iGroupIdx);
}

void CObject_Manager::Release()
{
	Delete_AllObjects();
}