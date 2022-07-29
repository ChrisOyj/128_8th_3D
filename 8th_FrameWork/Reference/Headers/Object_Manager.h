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
	CGameObject*			Get_StaticObj(const _uint& iKeyValue);
	list<CGameObject*>&		Get_ObjGroup(const _uint& iGroupIdx) { return m_pGameObjects[iGroupIdx]; }

public:
	void	Tick();
	void	Late_Tick();

public:
	void	Delete_All();
	void	Delete_Objects(const _uint& iGroupIdx);

private:
	map<_uint, CGameObject*>									m_mapStaticObj;
	list<CGameObject*>											m_pGameObjects[GR_END];

private:
	friend class CEvent_Manager;

	/* Must go through with Event_Manager to Add Object */

	void	Add_StaticObject(CGameObject* pGameObject, const _uint& iObjectID)	{m_mapStaticObj.emplace(iObjectID, pGameObject);}
	void	Add_Object(CGameObject* pGameObject, const _uint& iGroupIdx) { m_pGameObjects[iGroupIdx].push_back(pGameObject); }

private:
	void	Release();

};

END