#include "..\Public\Particle_Pop.h"



CParticle_Pop::CParticle_Pop()
{
}


CParticle_Pop::~CParticle_Pop()
{
}

CParticle_Pop * CParticle_Pop::Create(CParticleSystem * pSystem)
{
	CParticle_Pop * pParticle = new CParticle_Pop;
	pParticle->m_pSystem = pSystem;
	return pParticle;
}

void CParticle_Pop::SetUp(_float3 vPos, _float3 vDir, _float3 vScale, _float4 vColor, _float fSpeed, _float fChangeSpeed, _float fFadeOutSpeed, _float fJumpPower)
{
	__super::SetUp(vPos, vDir, vScale, vColor, fSpeed, fChangeSpeed, fFadeOutSpeed);
	m_fOriginY = vPos.y;

	m_fJumpPower = fJumpPower;
}

_bool CParticle_Pop::Tick(const _float & fTimeDelta)
{
	m_vColor.w -= m_fFadeOutSpeed * fTimeDelta;

	if (m_vColor.w < 0.1f)
		return false;

	m_fSpeed += m_fChangeSpeed * fTimeDelta;

	m_vPos += m_fSpeed * m_vDir * fTimeDelta;


	if (m_vPos.y < 0.f)
	{
		return false;
	}

	m_fJumpAcc += m_fJumpPlusAcc * fTimeDelta;

	float fY = m_fOriginY + ((m_fJumpPower * m_fJumpAcc) - (9.8f * 0.5f * m_fJumpAcc * m_fJumpAcc));
	m_vPos.y = fY;



	Make_WorldMatrix();


	return true;
}
