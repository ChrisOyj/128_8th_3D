#pragma once
#include "CState_Attack_Enemy.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CLeafHurricane_Attack_Enemy
	: public CState_Attack_Enemy
{
	DECLARE_STATE(CLeafHurricane_Attack_Enemy);
private:
	CLeafHurricane_Attack_Enemy();
	virtual ~CLeafHurricane_Attack_Enemy();

public:
	static CLeafHurricane_Attack_Enemy* Create();

public:
	virtual void	OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos) override;

public:
	// CState을(를) 통해 상속됨
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