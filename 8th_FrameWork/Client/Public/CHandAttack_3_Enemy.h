#pragma once
#include "CState_Attack_Enemy.h"

BEGIN(Client)
class CHandAttack_3_Enemy
	: public CState_Attack_Enemy
{
	DECLARE_STATE(CHandAttack_3_Enemy);
private:
	CHandAttack_3_Enemy();
	virtual ~CHandAttack_3_Enemy();

public:
	static CHandAttack_3_Enemy* Create();

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