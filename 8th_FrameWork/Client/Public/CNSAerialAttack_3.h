#pragma once
#include "CNinzaSword_Attack.h"

BEGIN(Client)
class CNSAerialAttack_3
	: public CNinzaSword_Attack
{
	DECLARE_STATE(CNSAerialAttack_3);
private:
	CNSAerialAttack_3();
	virtual ~CNSAerialAttack_3();

public:
	static CNSAerialAttack_3* Create();

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