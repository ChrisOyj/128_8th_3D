#pragma once
#include "CState.h"

BEGIN(Client)
class CDoubleJump_NightGuy
	: public CState
{
	DECLARE_STATE(CDoubleJump_NightGuy);
private:
	CDoubleJump_NightGuy();
	virtual ~CDoubleJump_NightGuy();

public:
	static CDoubleJump_NightGuy* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
};

END