#pragma once
#include "CState_Attack_Enemy.h"

BEGIN(Client)
class CHandAttack_1_Enemy
	: public CState_Attack_Enemy
{
	DECLARE_STATE(CHandAttack_1_Enemy);
private:
	CHandAttack_1_Enemy();
	virtual ~CHandAttack_1_Enemy();

public:
	static CHandAttack_1_Enemy* Create();

public:
	virtual void	OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos) override;



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