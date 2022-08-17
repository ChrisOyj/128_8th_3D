#include "GameObject.h"

#include "Transform.h"
#include "GameInstance.h"
#include "Physics.h"
#include "Collider.h"

#pragma region Constructor, Destructor
CGameObject::CGameObject()
{
	m_pTransform = CTransform::Create(1);
	Add_Component(m_pTransform);
	m_pTransform->Set_Owner(this);
}

CGameObject::CGameObject(const CGameObject & Prototype)
	: m_pParent(Prototype.m_pParent)
	, m_iGroupIndex(Prototype.m_iGroupIndex)
{
	//Deep Copy for Component
	for (auto& elem : Prototype.m_mapComponents)
	{
		for (auto& pComponent : elem.second)
		{
			CComponent* pCloneComponent = pComponent->Clone();
			pCloneComponent->Set_Owner(this);
			m_mapComponents[elem.first].push_back(pCloneComponent);
		}
	}

	m_pTransform = static_cast<CTransform*>(Get_Component<CTransform>().front());

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
HRESULT CGameObject::Start()
{
	for (auto& Complist : m_mapComponents)
	{
		for (auto& pComponent : Complist.second)
		{
			pComponent->Start();
		}
	}

	for (auto& pChild : m_pChildren)
		if (FAILED(pChild->Start()))
			return E_FAIL;

	return S_OK;
}
void CGameObject::Tick()
{
	//============ My Update ==============
	My_Tick();
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
	My_LateTick();
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

}
#pragma endregion

void CGameObject::Add_Child(CGameObject* pChild)
{
	pChild->Set_Parent(this);
	m_pChildren.push_back(pChild);
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
	for (auto mapIter = m_mapComponents.begin(); mapIter != m_mapComponents.end(); ++mapIter)
	{
		for (auto CompIter = mapIter->second.begin(); CompIter != mapIter->second.end(); ++CompIter)
		{
			SAFE_DESTROY((*CompIter));
		}

		mapIter->second.clear();
	}

	/*for (auto& CompList : m_mapComponents)
	{
		for (auto& pComponent : CompList.second)
		{
			SAFE_DESTROY(pComponent);
		}

		CompList.second.clear();
	}*/

	m_mapComponents.clear();

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

	for (auto& CompList : m_mapComponents)
	{
		for (auto& pComponent : CompList.second)
		{
			pComponent->Set_Enable(true);
		}
	}
}

void CGameObject::Call_Disable()
{
	for (auto& pChild : m_pChildren)
		pChild->Set_Enable(false);

	for (auto& CompList : m_mapComponents)
	{
		for (auto& pComponent : CompList.second)
		{
			pComponent->Set_Enable(false);
		}
	}
}

void CGameObject::Call_Dead()
{
	for (auto& pChild : m_pChildren)
		pChild->Set_Dead();

	for (auto& CompList : m_mapComponents)
	{
		for (auto& pComponent : CompList.second)
		{
			pComponent->Set_Dead();
		}
	}
}
