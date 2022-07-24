#pragma once
#include "Particle.h"


BEGIN(Engine)
class CParticle_Default
	: public CParticle
{
private:
	friend class CParticleSystem;
	CParticle_Default();
	~CParticle_Default();

public:
	static CParticle*	Create(CParticleSystem* pSystem);

public:
	virtual _bool Tick(const _float& fTimeDelta) override;


};

END