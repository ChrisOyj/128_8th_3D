#pragma once
#include "CState.h"

BEGIN(Client)
class CWalk_NightGuy
	: public CState
{
	DECLARE_STATE(CWalk_NightGuy);

private:
	CWalk_NightGuy();
	virtual ~CWalk_NightGuy();

public:
	static CWalk_NightGuy* Create();

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