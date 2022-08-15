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

HRESULT CObject_Manager::Regist_GameObject(CGameObject* pComponent, const _uint& iGroupIndex)
{
	if (iGroupIndex < 0 || iGroupIndex >= GR_END)
		return E_FAIL;

	m_pGameObjects[iGroupIndex].push_back(pComponent);

	return S_OK;
}

void CObject_Manager::Tick_GameObjects()
{
	for (auto StaticObjIter = m_mapStaticObj.begin(); StaticObjIter != m_mapStaticObj.end();)
	{
		if (!(*StaticObjIter).second->Is_Valid())
		{
			if ((*StaticObjIter).second->Is_Dead())
				StaticObjIter = m_mapStaticObj.erase(StaticObjIter);
			else
				++StaticObjIter;
		}
		else
		{
			(*StaticObjIter).second->Tick();
			++StaticObjIter;
		}
	}


	for (_uint i = 0; i < GR_END; ++i)
	{
		for (auto ObjIter = m_pGameObjects[i].begin(); ObjIter != m_pGameObjects[i].end();)
		{
			if (!(*ObjIter)->Is_Valid())
			{
				if ((*ObjIter)->Is_Dead())
					ObjIter = m_pGameObjects[i].erase(ObjIter);
				else
					++ObjIter;
			}
			else
			{
				(*ObjIter)->Tick();
				++ObjIter;
			}
		}
	}
}

void CObject_Manager::LateTick_GameObjects()
{
	for (auto& elem : m_mapStaticObj)
		elem.second->Late_Tick();

	for (_uint i = 0; i < GR_END; ++i)
	{
		for (auto& pGameObject : m_pGameObjects[i])
		{
			pGameObject->Late_Tick();
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