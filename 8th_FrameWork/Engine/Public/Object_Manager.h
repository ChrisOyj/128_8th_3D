#pragma once

/* ��ü���� ������ �����ϳ�. */
/* ������ü�� ã�Ƽ� �����Ͽ� ��(����)���� ��Ƽ� �����ϴ�.*/

#include "Engine_Defines.h"

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CObject_Manager final
{
	DECLARE_SINGLETON(CObject_Manager);

private:
	CObject_Manager();
	~CObject_Manager();

public:
	template <typename T>
	T*	Get_StaticObj()
	{
		auto iter = m_mapStaticObj.find(HASHCODE(T));
		if (iter == m_mapStaticObj.end())
			return nullptr;
		
		return (T*)iter->second;
	}

	list<CGameObject*>&		Get_ObjList() { return m_pGameObjects; }

public:
	void	Tick();
	void	Late_Tick();


public:
	void	Delete_All();
	void	Delete_Objects();

private:
	unordered_multimap<_hashcode, CGameObject*>					m_mapStaticObj;
	list<CGameObject*>											m_pGameObjects;

private:
	friend class CEvent_Manager;

	/* Must go through with Event_Manager to Add Object */

	void	Add_StaticObject(CGameObject* pGameObject, _hashcode hashcode)	{m_mapStaticObj.emplace(hashcode, pGameObject);}
	void	Add_Object(CGameObject* pGameObject) { m_pGameObjects.push_back(pGameObject); }

private:
	void	Release();

};

END