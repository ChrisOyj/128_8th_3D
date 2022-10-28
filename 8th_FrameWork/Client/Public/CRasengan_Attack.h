#pragma once
#include "CState_Attack.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CRasengan_Attack
	: public CState_Attack
{
	DECLARE_STATE(CRasengan_Attack);
private:
	CRasengan_Attack();
	virtual ~CRasengan_Attack();

public:
	virtual void OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos) override;

public:
	static CRasengan_Attack* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void OnExecute(CUnit* pOwner, CAnimator* pAnimator) override;


	// CState을(를) 통해 상속됨
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

};

END