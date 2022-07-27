#pragma once
#include "Particle.h"


BEGIN(Engine)
class CParticle_Pop
	: public CParticle
{
private:
	friend class CParticleSystem;
	CParticle_Pop();
	~CParticle_Pop();

public:
	static CParticle_Pop*	Create(CParticleSystem* pSystem);
	virtual void	SetUp(_float4 vPos, _float4 vDir, _float4 vScale, _float4 vColor, _float fSpeed, _float fChangeSpeed, _float fFadeOutSpeed, _float fJumpPower);


public:
	virtual void	Recycle(const _float4& vPos, const _float4& vDir) {
		m_vPos = vPos;
		m_vDir = vDir;
		m_vColor.w = 1.f;
		m_fSpeed = m_fOriginSpeed;
		m_fOriginY = m_vPos.y;
		m_fJumpAcc = 0.f;
	}

public:
	virtual _bool Tick(const _float& fTimeDelta) override;

private:
	_float	m_fOriginY = 0.f;
	_float	m_fJumpAcc = 0.f;
	_float	m_fJumpPlusAcc = 4.f;
	_float	m_fJumpPower = 10.f;


};

END