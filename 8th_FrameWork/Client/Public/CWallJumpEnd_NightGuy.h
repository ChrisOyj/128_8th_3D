#pragma once
#include "CState.h"

BEGIN(Client)
class CWallJumpEnd_NightGuy
	: public CState
{
	DECLARE_STATE(CWallJumpEnd_NightGuy);
private:
	CWallJumpEnd_NightGuy();
	virtual ~CWallJumpEnd_NightGuy();

public:
	static CWallJumpEnd_NightGuy* Create();

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