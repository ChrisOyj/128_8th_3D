#pragma once
#include "CState_Attack.h"

BEGIN(Client)
class CHandAerialAttack_3
	: public CState_Attack
{
	DECLARE_STATE(CHandAerialAttack_3);
private:
	CHandAerialAttack_3();
	virtual ~CHandAerialAttack_3();

public:
	static CHandAerialAttack_3* Create();


public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	_bool			m_bDoubleHitStart = false;
	_float			m_fDoubleTimeAcc = 0.f;


private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void		OnExecute(CUnit* pOwner, CAnimator* pAnimator);

private:
	virtual void		OnDisableCollider(CUnit* pOwner, CAnimator* pAnimator);


};

END