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
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

protected:
	//칼 집어넣는 프레임
	_uint	m_iBoneChangeFrame = 0;
	_bool	m_bBoneChanged = false;
	//칼 휘두르는 프레임
	_uint	m_iAttackExecuteFrame = 0;

	_uint	m_iSecondAttackExecuteFrame = 0;
	_bool	m_bSecondExcuted = false;
	_bool	m_bSecondDisableCol = false;
	//상태 바꾸는 프레임
	_uint	m_iStateChangableFrame = 0;
	//이번에 바꿀 뼈
	string	m_strRefBoneName;
	//원조 뼈
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