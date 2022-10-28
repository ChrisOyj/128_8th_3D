#pragma once
#include "CState_Attack.h"

BEGIN(Client)
class CHandAttack_Dash
	: public CState_Attack
{
	DECLARE_STATE(CHandAttack_Dash);
private:
	CHandAttack_Dash();
	virtual ~CHandAttack_Dash();

public:
	static CHandAttack_Dash* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;


private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void		OnExecute(CUnit* pOwner, CAnimator* pAnimator);

};

END