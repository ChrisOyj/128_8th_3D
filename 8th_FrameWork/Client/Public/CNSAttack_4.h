#pragma once
#include "CNinzaSword_Attack.h"

BEGIN(Client)
class CNSAttack_4
	: public CNinzaSword_Attack
{
	DECLARE_STATE(CNSAttack_4);
private:
	CNSAttack_4();
	virtual ~CNSAttack_4();

public:
	static CNSAttack_4* Create();

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