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
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

protected:
	//�浹ü ���°� �ð� ���
	_float		m_fAttackTimeAcc = 0.f;
	//�浹ü ���� �ð�
	_float		m_fColliderDisableTime = 0.2f;
	//�� �о�� ��
	_float		m_fKnockbackPower = 0.f;
	//�� ���� ��
	_float		m_fJumpPower = 0.f;
	//���� ������ ���� ��
	_float		m_fSpeed = 0.f;
	//�浹ü ����
	_uint		m_iColliderType = 0;
	//���� ����
	_uint		m_iHitType = 0;
	//Ÿ�� ã�� �Ÿ�
	_float		m_fTargetFindMaxDistance = 6.f;
	//������
	_float		m_fDamage = 2.f;

	_bool	m_bStopTime = false;
	_float	m_fStopTimeAcc = 0.f;



	//����� ����
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