#include "Prototype_Manager.h"
#include "GameObject.h"
#include "Component.h"

CPrototype_Manager::CPrototype_Manager()
{
}

CPrototype_Manager::~CPrototype_Manager()
{
	Release();
}

CGameObject* CPrototype_Manager::Clone_GameObject(const _uint& _iID)
{
	CGameObject* pPrototype = Find_GameObject_Prototype(_iID);

	if (!pPrototype)
	{
		Call_MsgBox(L"failed to find Prototype : CPrototype_Manager");
		return nullptr;
	}

	return pPrototype->Clone();
}

CComponent* CPrototype_Manager::Clone_Component(const _uint& _iID)
{
	CComponent* pPrototype = Find_Component_Prototype(_iID);

	if (!pPrototype)
	{
		Call_MsgBox(L"failed to find Prototype : CPrototype_Manager");
		return nullptr;
	}

	return pPrototype->Clone();
}

HRESULT CPrototype_Manager::Add_GameObject_Prototype(const _uint& _iID, CGameObject* pGameObject)
{
	CGameObject* pPrototype = Find_GameObject_Prototype(_iID);

	if (pPrototype)
	{
		Call_MsgBox(L"failed to Add GameObject : CPrototype_Manager");
		return E_FAIL;
	}

	m_GameObject_Prototypes.emplace(_iID, pGameObject);

	return S_OK;
}

HRESULT CPrototype_Manager::Add_Component_Prototype(const _uint& _iID, CComponent* pComponent)
{
	CComponent* pPrototype = Find_Component_Prototype(_iID);

	if (pPrototype)
	{
		Call_MsgBox(L"failed to Add Component : CPrototype_Manager");
		return E_FAIL;
	}

	m_Component_Prototypes.emplace(_iID, pComponent);

	return S_OK;
}

CGameObject* CPrototype_Manager::Find_GameObject_Prototype(const _uint& _iID)
{
	auto iter = m_GameObject_Prototypes.find(_iID);

	if (iter == m_GameObject_Prototypes.end())
		return nullptr;

	return iter->second;
}

CComponent* CPrototype_Manager::Find_Component_Prototype(const _uint& _iID)
{
	auto iter = m_Component_Prototypes.find(_iID);

	if (iter == m_Component_Prototypes.end())
		return nullptr;

	return iter->second;
}

void CPrototype_Manager::Release()
{
	for (auto iter = m_GameObject_Prototypes.begin(); iter != m_GameObject_Prototypes.end(); ++iter)
	{
		SAFE_DESTROY(iter->second);
	}

	m_GameObject_Prototypes.clear();

	for (auto iter = m_Component_Prototypes.begin(); iter != m_Component_Prototypes.end(); ++iter)
	{
		SAFE_DESTROY(iter->second);
	}

	m_Component_Prototypes.clear();
}
