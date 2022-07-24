#include "..\Public\Particle_Default.h"



CParticle_Default::CParticle_Default()
{
}


CParticle_Default::~CParticle_Default()
{
}

CParticle * CParticle_Default::Create(CParticleSystem * pSystem)
{
	CParticle_Default * pParticle = new CParticle_Default;
	pParticle->m_pSystem = pSystem;
	return pParticle;
}

_bool CParticle_Default::Tick(const _float& fTimeDelta)
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

	Make_WorldMatrix();


	return true;
}