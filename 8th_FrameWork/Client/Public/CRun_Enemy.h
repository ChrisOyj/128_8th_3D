#pragma once
#include "CState.h"

BEGIN(Client)
class CRun_Enemy
	: public CState
{
	DECLARE_STATE(CRun_Enemy);
private:
	CRun_Enemy();
	virtual ~CRun_Enemy();

public:
	static CRun_Enemy* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;


private:
	_float	m_fLoopSoundAcc = 0.f;
	_float	m_fSoundLoopTime = 0.15f;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
};

END