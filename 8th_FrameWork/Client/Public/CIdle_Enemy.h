#pragma once
#include "CState.h"

BEGIN(Client)
class CIdle_Enemy
	: public CState
{
	DECLARE_STATE(CIdle_Enemy);

private:
	CIdle_Enemy();
	virtual ~CIdle_Enemy();

public:
	static CIdle_Enemy* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
};

END