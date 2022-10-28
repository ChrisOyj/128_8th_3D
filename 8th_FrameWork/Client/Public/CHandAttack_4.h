#pragma once
#include "CState_Attack.h"

BEGIN(Client)
class CHandAttack_4
	: public CState_Attack
{
	DECLARE_STATE(CHandAttack_4);
private:
	CHandAttack_4();
	virtual ~CHandAttack_4();

public:
	static CHandAttack_4* Create();

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