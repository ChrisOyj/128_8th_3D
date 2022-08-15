#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CGameObject;

class CObject_Manager
{
	DECLARE_SINGLETON(CObject_Manager);

private:
	CObject_Manager();
	~CObject_Manager();

public:
	template <typename T>
	T* Get_StaticObj()
	{
		auto iter = m_mapStaticObj.find(HASHCODE(T));
		if (iter == m_mapStaticObj.end())
		{
			Call_MsgBox(L"Failed to find StaticObj : CObject_Manager");
			return nullptr;
		}

		return static_cast<T*>(iter->second);
	}
	list<CGameObject*>&		Get_ObjGroup(const _uint& iGroupIdx) { return m_pGameObjects[iGroupIdx]; }

public:
	HRESULT	Regist_GameObject(CGameObject* pGameObject, const _uint& iGroupIndex);

public:
	void	Tick_GameObjects();
	void	LateTick_GameObjects();

public:
	void	Check_Objects_Dead();

public:
	void	Delete_AllObjects();
	void	Delete_Objects(const _uint& iGroupIdx);

private:
	map<_hashcode, CGameObject*>									m_mapStaticObj;
	list<CGameObject*>											m_pGameObjects[GR_END];

private:
	friend class CEvent_Manager;

	/* Must go through with Event_Manager to Add Object */
	template <typename T>
	void	Add_StaticObject(CGameObject* pGameObject)
	{
		if (dynamic_cast<T*>(pGameObject) == nullptr)
			return;

		m_mapStaticObj.emplace(HASHCODE(T), pGameObject);
	}

	void	Add_StaticObject(CGameObject* pGameObject, _hashcode hcClassName)
	{
		m_mapStaticObj.emplace(hcClassName, pGameObject);
	}
	void	Add_Object(CGameObject* pGameObject, const _uint& iGroupIdx);

private:
	void	Release();

};

END