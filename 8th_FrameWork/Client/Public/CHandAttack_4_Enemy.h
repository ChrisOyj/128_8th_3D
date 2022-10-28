#pragma once
#include "CState_Attack_Enemy.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CHandAttack_4_Enemy
	: public CState_Attack_Enemy
{
	DECLARE_STATE(CHandAttack_4_Enemy);
private:
	CHandAttack_4_Enemy();
	virtual ~CHandAttack_4_Enemy();

public:
	static CHandAttack_4_Enemy* Create();

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