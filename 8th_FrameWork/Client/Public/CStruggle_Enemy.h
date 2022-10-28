#pragma once
#include "CState.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CStruggle_Enemy
	: public CState
{

private:
	CStruggle_Enemy();
	virtual ~CStruggle_Enemy();

	DECLARE_STATE(CStruggle_Enemy);

public:
	static CStruggle_Enemy* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual STATE_TYPE Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

public:
	list<CGameObject*>	m_EffectsList;
	_float		m_fParticleAcc = 0.f;
	_float		m_fParticleCreateTime = 0.3f;


private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
};

END