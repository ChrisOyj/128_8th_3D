#include "..\Public\Component.h"

CComponent::CComponent( )
{
}

void CComponent::Set_Enable(_bool bEnable)
{
	if (m_bEnable == bEnable)
		return;

	(m_bEnable = bEnable) ? OnEnable() : OnDisable();
}
