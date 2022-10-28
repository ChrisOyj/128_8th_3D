#include "CComponent_Manager.h"

#include "Component.h"

IMPLEMENT_SINGLETON(CComponent_Manager)


CComponent_Manager::CComponent_Manager()
{
}

CComponent_Manager::~CComponent_Manager()
{
}

HRESULT CComponent_Manager::Regist_Component(CComponent* pComponent, const _uint& iGroupIndex)
{
	if (iGroupIndex < 0 || iGroupIndex >= COM_GROUP_END)
		return E_FAIL;

	for (auto& elem : m_Components[iGroupIndex])
	{
		if (pComponent == elem)
			return S_OK;
	}

	m_Components[iGroupIndex].push_back(pComponent);

	return S_OK;
}

void CComponent_Manager::Clear_All()
{
	for (_uint i = 0; i < COM_GROUP_END; ++i)
		m_Components[i].clear();
}

void CComponent_Manager::Tick()
{
	for (_uint i = 0; i < COM_GROUP_END; ++i)
	{
		for (auto ComIter = m_Components[i].begin(); ComIter != m_Components[i].end();)
		{
			if (!(*ComIter)->Is_Valid())
			{
				ComIter = m_Components[i].erase(ComIter);
			}
			else
			{
				(*ComIter)->Tick();
				++ComIter;
			}
		}
	}
}

void CComponent_Manager::Late_Tick()
{
	for (_uint i = 0; i < COM_GROUP_END; ++i)
	{
		for (auto& pComponent : m_Components[i])
		{
			if (pComponent->Is_Valid())
				pComponent->Late_Tick();
		}
	}
}
