#pragma once
#include "CState.h"

BEGIN(Client)
class CWallJump_Enemy
	: public CState
{
	DECLARE_STATE(CWallJump_Enemy);
private:
	CWallJump_Enemy();
	virtual ~CWallJump_Enemy();

public:
	static CWallJump_Enemy* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	_float m_fTargetY = 0.f;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
};

END