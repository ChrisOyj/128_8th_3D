#pragma once
#include "CState.h"

BEGIN(Client)
class CLand_NightGuy
	: public CState
{
	DECLARE_STATE(CLand_NightGuy);
private:
	CLand_NightGuy();
	virtual ~CLand_NightGuy();

public:
	static CLand_NightGuy* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

	virtual void		OnExecute(CUnit* pOwner, CAnimator* pAnimator) { m_iAnimIndex = 37; }
};

END