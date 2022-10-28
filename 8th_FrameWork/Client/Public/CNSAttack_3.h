#pragma once
#include "CNinzaSword_Attack.h"

BEGIN(Client)
class CNSAttack_3
	: public CNinzaSword_Attack
{
	DECLARE_STATE(CNSAttack_3);
private:
	CNSAttack_3();
	virtual ~CNSAttack_3();

public:
	static CNSAttack_3* Create();

public:
	virtual void	OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos) override;

public:
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;


private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void		OnExecute(CUnit* pOwner, CAnimator* pAnimator);

};

END