#include "..\Public\Component.h"
#include "GameObject.h"

#include "CComponent_Manager.h"

CComponent::CComponent(_uint iGroupID)
	: m_iGroupID(iGroupID)
{
}

CComponent::~CComponent()
{
}

void CComponent::Set_Enable(_bool bEnable)
{
	if (m_bEnable == bEnable)
		return;

	(m_bEnable = bEnable) ? OnEnable() : OnDisable();
}

void CComponent::OnEnable()
{
	CComponent_Manager::Get_Instance()->Regist_Component(this, m_iGroupID);
}

void CComponent::OnDisable()
{
}
