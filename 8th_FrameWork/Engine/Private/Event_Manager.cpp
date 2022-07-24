#include "..\Public\Event_Manager.h"

#include "GameObject.h"
#include "Level.h"

#include "Object_Manager.h"
#include "Level_Manager.h"
#include "Load_Manager.h"

CEvent_Manager::CEvent_Manager()
{
}

CEvent_Manager::~CEvent_Manager()
{
	m_vecDead.clear();
	m_vecEvent.clear();

}

void CEvent_Manager::Tick()
{
	/* Delete all Dead_Object from prev Frame. */
	for (size_t i = 0; i < m_vecDead.size(); ++i)
	{
		if (nullptr != m_vecDead[i])
			m_vecDead[i]->Destory_Instance();
	}
	m_vecDead.clear();

	for (size_t i = 0; i < m_vecEvent.size(); ++i)
	{
		Execute(m_vecEvent[i]);
	}
	m_vecEvent.clear();
	
}

void CEvent_Manager::Delete_GameObject(CGameObject * pGameObject)
{
	Add_Event(EVENT_DELETE_OBJECT, (DWORD_PTR)pGameObject);
}

void CEvent_Manager::Create_GameObject(CGameObject * pGameObject)
{
	Add_Event(EVENT_CREATE_OBJECT, (DWORD_PTR)pGameObject);
}

void CEvent_Manager::Enable_GameObject(CGameObject * pGameObject)
{
	Add_Event(EVENT_ENABLE_OBJECT, (DWORD_PTR)pGameObject);
}

void CEvent_Manager::Disable_GameObject(CGameObject * pGameObject)
{
	Add_Event(EVENT_DISABLE_OBJECT, (DWORD_PTR)pGameObject);
}

void CEvent_Manager::Create_StaticObject(CGameObject * pGameObject, _hashcode hashcode)
{
	Add_Event(EVENT_CREATE_STATIC, (DWORD_PTR)pGameObject, (DWORD_PTR)hashcode);

}

void CEvent_Manager::Change_Level(_uint iLevelID, CLevel * pLevel)
{
	Add_Event(EVENT_CHANGE_LEVEL, (DWORD_PTR)iLevelID, (DWORD_PTR)pLevel);
}

void CEvent_Manager::Add_Event(const EVENT_ID & eEven, const DWORD_PTR & lParam, const DWORD_PTR & wParam)
{
	m_vecEvent.push_back(EVENT(eEven, lParam, wParam));
}

void CEvent_Manager::Execute(const EVENT & tEvent)
{
	switch (tEvent.eEven)
	{
	default:
		break;

	case EVENT_CREATE_OBJECT:
	{
		CGameObject* pGameObject =(CGameObject*)(tEvent.lParam);
		CObject_Manager::Get_Instance()->Add_Object(pGameObject);
		pGameObject->Start_Components();
	}
	break;

	case EVENT_CREATE_STATIC:
	{
		CGameObject* pGameObject = (CGameObject*)(tEvent.lParam);
		_hashcode	hashcode = static_cast<_hashcode>(tEvent.wParam);
		CObject_Manager::Get_Instance()->Add_StaticObject(pGameObject, hashcode);
		pGameObject->Start_Components();

	}
	break;

	case EVENT_DELETE_OBJECT:
	{
		CGameObject* pGameObject = (CGameObject*)(tEvent.lParam);
		if (pGameObject->IsDead())
			return;

		pGameObject->Set_Dead();

		/* Prevent Overlapped pointer */
		for (auto& elem : m_vecDead)
		{
			if (elem == pGameObject)
				return;
		}

		m_vecDead.push_back(pGameObject);
	}
	break;

	case EVENT_ENABLE_OBJECT:
	{
		CGameObject* pGameObject = (CGameObject*)(tEvent.lParam);
		pGameObject->Set_Enable(true);
	}

	break;

	case EVENT_DISABLE_OBJECT:
	{
		CGameObject* pGameObject = (CGameObject*)(tEvent.lParam);
		pGameObject->Set_Enable(false);
	}
		break;

	case EVENT_CHANGE_LEVEL:
	{
		CLevel*	pLevel = (CLevel*)(tEvent.wParam);
		_uint	iLevelID = static_cast<_uint>(tEvent.lParam);
		CLevel_Manager::Get_Instance()->Open_Level(iLevelID, pLevel);
	}
	break;

	}
}
