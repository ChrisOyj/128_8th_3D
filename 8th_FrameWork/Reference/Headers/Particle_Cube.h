#pragma once
#include "Particle.h"


BEGIN(Engine)
class CParticle_Cube
	: public CParticle
{
private:
	friend class CParticleSystem;
	CParticle_Cube();
	~CParticle_Cube();

public:
	static CParticle_Cube*	Create(CParticleSystem* pSystem);
	virtual void	SetUp(_float4 vPos, _float4 vDir, _float4 vScale, _float4 vColor, _float fSpeed, _float fChangeSpeed, _float fFadeOutSpeed, _float fJumpPower, _float fTurnSpeed);


public:
	virtual void	Recycle(const _float4& vPos, const _float4& vDir) {
		m_vPos = vPos;
		m_vDir = vDir;
		m_vColor.w = 1.f;
		m_fSpeed = m_fOriginSpeed;
		m_fTurnSpeed = m_fOriginTurnSpeed;
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
	_float  m_fTurnSpeed = 0.f;
	_float	m_fTurnChangeSpeed = 10.f;
	_float  m_fOriginTurnSpeed = 0.f;

private:
	virtual void	Make_WorldMatrix() override;



};

END