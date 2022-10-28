#pragma once
#include "CState_Attack.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CNinzaSword_Attack abstract
	: public CState_Attack
{
protected:
	CNinzaSword_Attack();
	virtual ~CNinzaSword_Attack();

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
	//Į ����ִ� ������
	_uint	m_iBoneChangeFrame = 0;
	_bool	m_bBoneChanged = false;
	//Į �ֵθ��� ������
	_uint	m_iAttackExecuteFrame = 0;

	_uint	m_iSecondAttackExecuteFrame = 0;
	_bool	m_bSecondExcuted = false;
	_bool	m_bSecondDisableCol = false;
	//���� �ٲٴ� ������
	_uint	m_iStateChangableFrame = 0;
	//�̹��� �ٲ� ��
	string	m_strRefBoneName;
	//���� ��
	string	m_strOriginRefBoneName;


protected:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual	void	OnExecute(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void		OnDisableCollider(CUnit* pOwner, CAnimator* pAnimator);


private:
	_bool			m_bDisableCollider = false;
	_bool			m_bOnce = false;
};

END