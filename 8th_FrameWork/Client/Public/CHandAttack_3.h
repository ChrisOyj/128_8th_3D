#pragma once
#include "CState_Attack.h"

BEGIN(Client)
class CHandAttack_3
	: public CState_Attack
{
	DECLARE_STATE(CHandAttack_3);
private:
	CHandAttack_3();
	virtual ~CHandAttack_3();

public:
	static CHandAttack_3* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;


private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void		OnExecute(CUnit* pOwner, CAnimator* pAnimator);

private:
	virtual void		OnDisableCollider(CUnit* pOwner, CAnimator* pAnimator) override;


};

END