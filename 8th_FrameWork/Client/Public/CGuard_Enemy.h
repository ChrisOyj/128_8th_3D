#pragma once
#include "CState.h"

BEGIN(Client)
class CGuard_Enemy
	: public CState
{
	DECLARE_STATE(CGuard_Enemy);
private:
	CGuard_Enemy();
	virtual ~CGuard_Enemy();

public:
	static CGuard_Enemy* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual STATE_TYPE Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
};

END