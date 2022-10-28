#pragma once
#include "CState.h"

BEGIN(Client)
class CThrow_Enemy
	: public CState
{
	DECLARE_STATE(CThrow_Enemy);
private:
	CThrow_Enemy();
	virtual ~CThrow_Enemy();

public:
	static CThrow_Enemy* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;


private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void OnExecute(CUnit* pOwner, CAnimator* pAnimator) override;
};

END