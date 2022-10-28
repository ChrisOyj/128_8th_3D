#pragma once
#include "CState.h"

BEGIN(Client)
class CDash_NightGuy
	: public CState
{
	DECLARE_STATE(CDash_NightGuy);
private:
	CDash_NightGuy();
	virtual ~CDash_NightGuy();

public:
	static CDash_NightGuy* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;


private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual void		OnExecute(CUnit* pOwner, CAnimator* pAnimator);
	virtual void		TickExecute(CUnit* pOwner, CAnimator* pAnimator);
};

END