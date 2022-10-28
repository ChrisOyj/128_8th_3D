#pragma once
#include "CState.h"

BEGIN(Client)
class CAirDashEnd
	: public CState
{
	DECLARE_STATE(CAirDashEnd);
private:
	CAirDashEnd();
	virtual ~CAirDashEnd();

public:
	static CAirDashEnd* Create();

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