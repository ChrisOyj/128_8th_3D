#pragma once
#include "CState.h"

BEGIN(Client)
class CRun_End_NightGuy
	: public CState
{
	DECLARE_STATE(CRun_End_NightGuy);
private:
	CRun_End_NightGuy();
	virtual ~CRun_End_NightGuy();

public:
	static CRun_End_NightGuy* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
};

END