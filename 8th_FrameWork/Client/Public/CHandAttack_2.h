#pragma once
#include "CState_Attack.h"

BEGIN(Client)
class CHandAttack_2
	: public CState_Attack
{
	DECLARE_STATE(CHandAttack_2);
private:
	CHandAttack_2();
	virtual ~CHandAttack_2();

public:
	static CHandAttack_2* Create();

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