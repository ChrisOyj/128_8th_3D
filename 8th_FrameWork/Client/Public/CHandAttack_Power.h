#pragma once
#include "CState_Attack.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CHandAttack_Power
	: public CState_Attack
{
	DECLARE_STATE(CHandAttack_Power);
private:
	CHandAttack_Power();
	virtual ~CHandAttack_Power();

public:
	static CHandAttack_Power* Create();

public:
	virtual void	OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos) override;

public:
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	_bool	m_bColHit = false;
	_float	m_fMyTimeAcc = 0.f;


private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void		OnExecute(CUnit* pOwner, CAnimator* pAnimator);

};

END