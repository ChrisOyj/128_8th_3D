#include "Prototype_Manager.h"
#include "Obj.h"

CPrototype_Manager::CPrototype_Manager()
{
}

CPrototype_Manager::~CPrototype_Manager()
{
	Release();
}

CObj * CPrototype_Manager::Get_Clone(_hashcode hashcode)
{
	CObj*	pPrototype = Find_Prototype(hashcode);

	if (!pPrototype)
	{
		MSG_BOX("failed to find Prototype : CPrototype_Manager");
		return nullptr;
	}

	return pPrototype->Clone();
}

CObj * CPrototype_Manager::Find_Prototype(_hashcode hashcode)
{
	auto iter = m_Prototypes.find(hashcode);

	if (iter == m_Prototypes.end())
		return nullptr;

	return iter->second;
}

void CPrototype_Manager::Release()
{
	for (auto iter = m_Prototypes.begin(); iter != m_Prototypes.end(); ++iter)
	{
		iter->second->Destory_Instance();
	}

	m_Prototypes.clear();
}
