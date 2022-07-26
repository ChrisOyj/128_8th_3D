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

CGameObject* CPrototype_Manager::Clone_GameObject(_hashcode hashcode)
{
	CGameObject* pPrototype = Find_GameObject_Prototype(hashcode);

	if (!pPrototype)
	{
		Call_MsgBox(L"failed to find Prototype : CPrototype_Manager");
		return nullptr;
	}

	return pPrototype->Clone();
}

CComponent* CPrototype_Manager::Clone_Component(_hashcode hashcode)
{
	CComponent* pPrototype = Find_Component_Prototype(hashcode);

	if (!pPrototype)
	{
		Call_MsgBox(L"failed to find Prototype : CPrototype_Manager");
		return nullptr;
	}

	return pPrototype->Clone();
}

CGameObject* CPrototype_Manager::Find_GameObject_Prototype(_hashcode hashcode)
{
	auto iter = m_GameObject_Prototypes.find(hashcode);

	if (iter == m_GameObject_Prototypes.end())
		return nullptr;

	return iter->second;
}

CComponent* CPrototype_Manager::Find_Component_Prototype(_hashcode hashcode)
{
	auto iter = m_Component_Prototypes.find(hashcode);

	if (iter == m_Component_Prototypes.end())
		return nullptr;

	return iter->second;
}


void CPrototype_Manager::Release()
{
	for (auto iter = m_GameObject_Prototypes.begin(); iter != m_GameObject_Prototypes.end(); ++iter)
	{
		delete iter->second;
	}

	m_GameObject_Prototypes.clear();

	for (auto iter = m_Component_Prototypes.begin(); iter != m_Component_Prototypes.end(); ++iter)
	{
		delete iter->second;
	}

	m_Component_Prototypes.clear();
}
