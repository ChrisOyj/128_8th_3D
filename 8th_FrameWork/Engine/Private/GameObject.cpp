#include "GameObject.h"

#include "Transform.h"
#include "GameInstance.h"
#include "Physics.h"
#include "Collider.h"

#pragma region Constructor, Destructor
CGameObject::CGameObject()
{
	m_pTransform = static_cast<CTransform*>(CLONE_COMPONENT(TRANSFORM_PROTOTYPE_ID));
	Add_Component(m_pTransform);
}

CGameObject::CGameObject(const CGameObject & Prototype)
	: m_pParent(Prototype.m_pParent)
	, m_iID(Prototype.m_iID)
	, m_iGroupIndex(Prototype.m_iGroupIndex)
{
	//Deep Copy for Component
	for (auto& elem : Prototype.m_pComponents)
		Add_Component(elem->Clone());

	m_pTransform = static_cast<CTransform*>(m_pComponents.front());
	if (!Get_Component<CCollider>().empty())
	{
		m_pCollider = Get_Component<CCollider>()[0];
	}

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
	CallBack_CollisionEnter.Clear();
	CallBack_CollisionExit.Clear();
	CallBack_CollisionStay.Clear();
	CallBack_TimerEvent.Clear();
	CallBack_PickingEvent.Clear();
	//============ My Update ==============

	//for (auto& elem : m_pComponents)
		//elem->Tick();
	//======================================


	//=============== Children's Update ==============
	//for (auto& elem : m_pChildren)
	//{
	//	if (elem->Is_Valid())
	//		elem->Tick();
	//}
	//==============================================
}

void CGameObject::Late_Tick()
{
	////============ My Update ==============
	//for (auto iter = m_pComponents.begin(); iter != m_pComponents.end();)
	//{
	//	CComponent* pComponent = *iter;

	//	if (pComponent->Is_Dead())
	//		iter = m_pComponents.erase(iter);
	//	else
	//	{
	//		if (!pComponent->Is_Disable())
	//			pComponent->Late_Tick();

	//		++iter;
	//	}
	//}
	////======================================

	////=============== Children's Update ==============
	//for (auto iter = m_pChildren.begin(); iter != m_pChildren.end();)
	//{
	//	//if Instance is dead, get rid of it from list m_pChildren
	//	CGameObject* pChild = *iter;

	//	if (pChild->Is_Dead())
	//		iter = m_pChildren.erase(iter);
	//	else
	//	{
	//		if (!pChild->Is_Disable())
	//			pChild->Late_Tick();

	//		++iter;
	//	}
	//}
	////==============================================
	//My_TimerTick();

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

void CGameObject::OnEnable()
{
	//CObject_Manager::Get_Instance()->Regist_GameObject(this, m_iGroupIndex);
	Call_Enable();
}

void CGameObject::OnDisable()
{
	Call_Disable();
}

void CGameObject::Release()
{
	for (auto iter = m_pComponents.begin(); iter != m_pComponents.end(); ++iter)
	{
		CComponent* pComponent = *iter;
		SAFE_DESTROY(pComponent);
	}

	m_pComponents.clear();

	for (auto iter = m_pChildren.begin(); iter != m_pChildren.end(); ++iter)
	{
		CGameObject* pChild = *iter;
		SAFE_DESTROY(pChild);
	}

	m_pChildren.clear();
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

void CGameObject::Call_Dead()
{
	for (auto& pChild : m_pChildren)
		pChild->Set_Dead();

	for (auto& pComponent : m_pComponents)
		pComponent->Set_Dead();
}
