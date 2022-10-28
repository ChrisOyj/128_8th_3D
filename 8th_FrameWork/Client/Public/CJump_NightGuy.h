#pragma once
#include "CState.h"

BEGIN(Client)
class CJump_NightGuy
	: public CState
{
	DECLARE_STATE(CJump_NightGuy);
private:
	CJump_NightGuy();
	virtual ~CJump_NightGuy();

public:
	static CJump_NightGuy* Create();

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