#include "GameObject.h"

#include "Transform.h"
#include "GameInstance.h"
#include "Physics.h"
#include "Collider.h"

#pragma region Constructor, Destructor
CGameObject::CGameObject()
{
	/* GameObject must needs a Transform Component. */

	m_pTransform = CTransform::Create();

	Add_Component(m_pTransform);
}

CGameObject::CGameObject(const CGameObject & Prototype)
	: m_pParent(Prototype.m_pParent)
{
	//Deep Copy for Component
	for (auto& elem : Prototype.m_pComponents)
		Add_Component(elem->Clone());

	m_pTransform = static_cast<CTransform*>(m_pComponents.front());

	//Deep Copy for Children
	for (auto& elem : Prototype.m_pChildren)
		Add_Child(elem->Clone());
}

CGameObject::~CGameObject()
{
	Release();
}
#pragma endregion

CGameObject * CGameObject::Get_RootParent()
{
	if (!m_pParent)
		return this;
	else
	{
		return m_pParent->Get_RootParent();
	}
}

void CGameObject::Set_Enable(_bool bEnable)
{
	if (m_bEnable == bEnable)
		return;

	(m_bEnable = bEnable) ? OnEnable() : OnDisable();
}

#pragma region Tick, Late_Tick
void CGameObject::Tick()
{
	//============ My Update ==============

	for (auto& elem : m_pComponents)
		elem->Tick();
	//======================================


	//=============== Children's Update ==============
	for (auto& elem : m_pChildren)
	{
		if (elem->Is_Valid())
			elem->Tick();
	}
	//==============================================
}

void CGameObject::Late_Tick()
{
	//============ My Update ==============
	for (auto iter = m_pComponents.begin(); iter != m_pComponents.end();)
	{
		CComponent* pComponent = *iter;

		if (pComponent->Is_Dead())
			iter = m_pComponents.erase(iter);
		else
		{
			if (!pComponent->Is_Disable())
				pComponent->Late_Tick();

			++iter;
		}
	}
	//======================================

	//=============== Children's Update ==============
	for (auto iter = m_pChildren.begin(); iter != m_pChildren.end();)
	{
		//if Instance is dead, get rid of it from list m_pChildren
		CGameObject* pChild = *iter;

		if (pChild->Is_Dead())
			iter = m_pChildren.erase(iter);
		else
		{
			if (!pChild->Is_Disable())
				pChild->Late_Tick();

			++iter;
		}
	}
	//==============================================
	My_TimerTick();

}
#pragma endregion

void CGameObject::Add_Component(CComponent* pComponent)
{
	pComponent->Set_Owner(this);
	m_pComponents.push_back(pComponent);
}

void CGameObject::Add_Child(CGameObject* pChild)
{
	pChild->Set_Parent(this);
	m_pChildren.push_back(pChild);
}

void CGameObject::Start_Components()
{
	for (auto& pComp : m_pComponents)
		pComp->Start();

	for (auto& pChild : m_pChildren)
		pChild->Start_Components();
}

void CGameObject::My_TimerTick()
{
	for (auto TimerIter = m_TimerEvents.begin(); TimerIter != m_TimerEvents.end();)
	{
		TimerIter->fCurTime -= fDT;

		if (TimerIter->fCurTime <= 0.f)
		{
			OnTimerEvent(TimerIter->iEventNum);

			if (TimerIter->bLoop)
			{
				TimerIter->fCurTime = TimerIter->fOriginTime;
				++TimerIter;
				continue;
			}
			else
			{
				TimerIter = m_TimerEvents.erase(TimerIter);
				continue;
			}
		}

		++TimerIter;
	}
}

void CGameObject::Add_Timer(const _float & fTime, const _uint & iEventNum, _bool bLoop)
{
	TIMER_EVENT	tTimerEvent;
	tTimerEvent.fOriginTime = fTime;
	tTimerEvent.fCurTime = tTimerEvent.fOriginTime;
	tTimerEvent.iEventNum = iEventNum;
	tTimerEvent.bLoop = bLoop;

	m_TimerEvents.push_back(tTimerEvent);
}

void CGameObject::Release()
{
	for (auto iter = m_pComponents.begin(); iter != m_pComponents.end(); ++iter)
		delete (*iter);

	m_pComponents.clear();

	for (auto iter = m_pChildren.begin(); iter != m_pChildren.end(); ++iter)
		delete (*iter);

	m_pChildren.clear();
}

void CGameObject::Call_CollisionEnter(CGameObject* pGameObject, const _uint& iColType, _float4 vColPoint)
{
	for (auto& pChild : m_pChildren)
		pChild->OnCollisionEnter(pGameObject, iColType, vColPoint);
}

void CGameObject::Call_CollisionStay(CGameObject* pGameObject, const _uint& iColType)
{
	for (auto& pChild : m_pChildren)
		pChild->OnCollisionStay(pGameObject, iColType);
}

void CGameObject::Call_CollisionExit(CGameObject* pGameObject, const _uint& iColType)
{
	for (auto& pChild : m_pChildren)
		pChild->OnCollisionExit(pGameObject, iColType);
}

void CGameObject::Call_PickingEvent(const _float4& vPickedPos, const _float4& vPickedNormal)
{
	for (auto& pChild : m_pChildren)
		pChild->Call_PickingEvent(vPickedPos, vPickedNormal);
}

void CGameObject::Call_TimerEvent(const _uint& iEventNum)
{
	for (auto& pChild : m_pChildren)
		pChild->Call_TimerEvent(iEventNum);
}

void CGameObject::Call_Enable()
{
	for (auto& pChild : m_pChildren)
		pChild->Set_Enable(true);

	for (auto& pComponent : m_pComponents)
		pComponent->Set_Enable(true);
}

void CGameObject::Call_Disable()
{
	for (auto& pChild : m_pChildren)
		pChild->Set_Enable(false);

	for (auto& pComponent : m_pComponents)
		pComponent->Set_Enable(false);
}
