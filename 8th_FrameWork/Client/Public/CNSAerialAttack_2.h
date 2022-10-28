#pragma once
#include "CNinzaSword_Attack.h"

BEGIN(Client)
class CNSAerialAttack_2
	: public CNinzaSword_Attack
{
	DECLARE_STATE(CNSAerialAttack_2);
private:
	CNSAerialAttack_2();
	virtual ~CNSAerialAttack_2();

public:
	static CNSAerialAttack_2* Create();

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