#include "..\Public\Component.h"
#include "GameObject.h"

CComponent::CComponent(CGameObject* pOwner)
	: m_pOwner(pOwner)
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
