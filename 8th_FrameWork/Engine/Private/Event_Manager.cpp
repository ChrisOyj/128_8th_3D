#include "..\Public\Event_Manager.h"

#include "GameObject.h"
#include "Component.h"
#include "Level.h"

#include "Object_Manager.h"
#include "Level_Manager.h"

CEvent_Manager::CEvent_Manager()
{
}

CEvent_Manager::~CEvent_Manager()
{
	m_vecDeadObjects.clear();
	m_vecDeadComponents.clear();
	m_vecEvent.clear();

}

void CEvent_Manager::Tick()
{
	for (size_t i = 0; i < m_vecDeadComponents.size(); ++i)
	{
		if (nullptr != m_vecDeadComponents[i])
			delete m_vecDeadComponents[i];
	}

	m_vecDeadComponents.clear();


	/* Delete all Dead_Object from prev Frame. */
	for (size_t i = 0; i < m_vecDeadObjects.size(); ++i)
	{
		if (nullptr != m_vecDeadObjects[i])
			delete m_vecDeadObjects[i];
	}

	m_vecDeadObjects.clear();


	

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

void CEvent_Manager::Create_GameObject(CGameObject * pGameObject, const _uint& iGroupIdx)
{
	Add_Event(EVENT_CREATE_OBJECT, (DWORD_PTR)pGameObject, (DWORD_PTR)iGroupIdx);
}

void CEvent_Manager::Delete_Component(CComponent* pComponent, CGameObject* pGameObject)
{
	Add_Event(EVENT_DELETE_COMPONENT, (DWORD_PTR)pComponent, (DWORD_PTR)pGameObject);
}

void CEvent_Manager::Create_Component(CComponent* pComponent, CGameObject* pGameObject)
{
	Add_Event(EVENT_CREATE_COMPONENT, (DWORD_PTR)pComponent, (DWORD_PTR)pGameObject);
}

void CEvent_Manager::Enable_GameObject(CGameObject * pGameObject)
{
	Add_Event(EVENT_ENABLE_OBJECT, (DWORD_PTR)pGameObject);
}

void CEvent_Manager::Disable_GameObject(CGameObject * pGameObject)
{
	Add_Event(EVENT_DISABLE_OBJECT, (DWORD_PTR)pGameObject);
}

void CEvent_Manager::Disable_Component(CComponent* pComponent)
{
	Add_Event(EVENT_DISABLE_COMPONENT, (DWORD_PTR)pComponent);
}

void CEvent_Manager::Enable_Component(CComponent* pComponent)
{
	Add_Event(EVENT_ENABLE_COMPONENT, (DWORD_PTR)pComponent);
}

void CEvent_Manager::Create_StaticObject(CGameObject * pGameObject, const _uint& iObjectID)
{
	Add_Event(EVENT_CREATE_STATIC, (DWORD_PTR)pGameObject, (DWORD_PTR)iObjectID);

}

void CEvent_Manager::Change_Level(CLevel* pLevel)
{
	Add_Event(EVENT_CHANGE_LEVEL, (DWORD_PTR)pLevel);
}

void CEvent_Manager::Add_Event(const EVENT_TYPE & eEven, const DWORD_PTR & lParam, const DWORD_PTR & wParam)
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
		CGameObject* pGameObject = (CGameObject*)(tEvent.lParam);
		_uint iGroupIdx = (_uint)(tEvent.wParam);
		CObject_Manager::Get_Instance()->Add_Object(pGameObject, iGroupIdx);
		pGameObject->Start_Components();

	}
	break;

	case EVENT_CREATE_COMPONENT:
	{
		CComponent* pComponent = (CComponent*)(tEvent.lParam);
		CGameObject* pGameObject = (CGameObject*)(tEvent.wParam);
		pGameObject->Add_Component(pComponent);
		pComponent->Start();
	}
	break;

	case EVENT_CREATE_STATIC:
	{
		CGameObject* pGameObject = (CGameObject*)(tEvent.lParam);
		_uint	iObjectID = static_cast<_uint>(tEvent.wParam);
		CObject_Manager::Get_Instance()->Add_StaticObject(pGameObject, iObjectID);
		pGameObject->Start_Components();

	}
	break;

	case EVENT_DELETE_OBJECT:
	{
		CGameObject* pGameObject = (CGameObject*)(tEvent.lParam);
		if (pGameObject->Is_Dead())
			return;

		pGameObject->Set_Dead();

		/* Prevent Overlapped pointer */
		/*for (auto& elem : m_vecDead)
		{
			if (elem == pGameObject)
				return;
		}*/

		m_vecDeadObjects.push_back(pGameObject);
	}
	break;

	case EVENT_DELETE_COMPONENT:
	{
		CComponent* pComponent = (CComponent*)(tEvent.lParam);
		if (pComponent->Is_Dead())
			return;

		pComponent->Set_Dead();

		/* Prevent Overlapped pointer */
		/*for (auto& elem : m_vecDead)
		{
			if (elem == pGameObject)
				return;
		}*/

		m_vecDeadComponents.push_back(pComponent);
	}
	break;

	case EVENT_ENABLE_OBJECT:
	{
		CGameObject* pGameObject = (CGameObject*)(tEvent.lParam);
		pGameObject->Set_Enable(true);
		pGameObject->Call_Enable();
	}

	break;

	case EVENT_ENABLE_COMPONENT:
	{
		CComponent* pComponent = (CComponent*)(tEvent.lParam);
		pComponent->Set_Enable(true);
	}

	break;

	case EVENT_DISABLE_OBJECT:
	{
		CGameObject* pGameObject = (CGameObject*)(tEvent.lParam);
		pGameObject->Set_Enable(false);
		pGameObject->Call_Disable();

	}
		break;

	case EVENT_DISABLE_COMPONENT:
	{
		CComponent* pComponent = (CComponent*)(tEvent.lParam);
		pComponent->Set_Enable(false);
	}

	break;

	case EVENT_CHANGE_LEVEL:
	{
		CLevel*	pLevel = (CLevel*)(tEvent.lParam);
		CLevel_Manager::Get_Instance()->Enter_Level(pLevel);
	}
	break;

	}
}
