#include "Prototype_Manager.h"
#include "GameObject.h"
#include "Component.h"

#include "Transform.h"

#include "GameInstance.h"

IMPLEMENT_SINGLETON(CPrototype_Manager)



CPrototype_Manager::CPrototype_Manager()
{
}

CPrototype_Manager::~CPrototype_Manager()
{
	Release();
}

HRESULT CPrototype_Manager::Initialize()
{

	return S_OK;
}

CGameObject* CPrototype_Manager::Clone_GameObject(_hashcode hcClassName)
{
	auto iter = m_GameObject_Prototypes.find(hcClassName);

	if (iter == m_GameObject_Prototypes.end())
		return nullptr;

	return iter->second->Clone();
}

CComponent* CPrototype_Manager::Clone_Component(_hashcode hcClassName)
{
	auto iter = m_Component_Prototypes.find(hcClassName);

	if (iter == m_Component_Prototypes.end())
		return nullptr;

	return iter->second->Clone();
}

HRESULT CPrototype_Manager::Add_GameObject_Prototype(CGameObject* pGameObject, _hashcode _hcCode)
{
	if (Find_GameObject_Prototype(_hcCode))
		return E_FAIL;

	m_GameObject_Prototypes.emplace(_hcCode, pGameObject);

	return S_OK;
}

void CPrototype_Manager::Delete_GameObject_Prototypes()
{
	for (auto elemPair : m_GameObject_Prototypes)
		SAFE_DESTROY(elemPair.second);

	m_GameObject_Prototypes.clear();
}

void CPrototype_Manager::Delete_Component_Prototypes()
{
	for (auto elemPair : m_Component_Prototypes)
		SAFE_DESTROY(elemPair.second);

	m_Component_Prototypes.clear();
}

CGameObject* CPrototype_Manager::Find_GameObject_Prototype(_hashcode _hcCode)
{
	auto iter = m_GameObject_Prototypes.find(_hcCode);

	if (iter == m_GameObject_Prototypes.end())
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
