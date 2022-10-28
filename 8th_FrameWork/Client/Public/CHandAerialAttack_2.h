#pragma once
#include "CState_Attack.h"

BEGIN(Client)
class CHandAerialAttack_2
	: public CState_Attack
{
	DECLARE_STATE(CHandAerialAttack_2);
private:
	CHandAerialAttack_2();
	virtual ~CHandAerialAttack_2();

public:
	static CHandAerialAttack_2* Create();


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