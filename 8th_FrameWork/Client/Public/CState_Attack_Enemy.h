#pragma once
#include "CState.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CState_Attack_Enemy
	: public CState
{
protected:
	CState_Attack_Enemy();
	virtual ~CState_Attack_Enemy();

public:
	virtual void	OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos);

public:
	STATE_TYPE	Get_HitState() { return m_eHitStateType; }

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

protected:
	//충돌체 끄는거 시간 재기
	_float		m_fAttackTimeAcc = 0.f;
	//충돌체 지속 시간
	_float		m_fColliderDisableTime = 0.2f;
	//적 밀어내는 힘
	_float		m_fKnockbackPower = 0.f;
	//적 띄우는 힘
	_float		m_fJumpPower = 0.f;
	//내가 앞으로 가는 힘
	_float		m_fSpeed = 0.f;
	//충돌체 부위
	_uint		m_iColliderType = 0;
	//공격 판정
	_uint		m_iHitType = 0;
	//타겟 찾는 거리
	_float		m_fTargetFindMaxDistance = 6.f;
	//데미지
	_float		m_fDamage = 2.f;

	_bool	m_bStopTime = false;
	_float	m_fStopTimeAcc = 0.f;



	//연결될 상태
	STATE_TYPE	m_eHitStateType = STATE_HIT_GROUND_PLAYER;


protected:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void		OnExecute(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void		TickExecute(CUnit* pOwner, CAnimator* pAnimator) override;

protected:
	virtual void		OnDisableCollider(CUnit* pOwner, CAnimator* pAnimator);

private:
	_bool			m_bDisableCollider = false;

};

END