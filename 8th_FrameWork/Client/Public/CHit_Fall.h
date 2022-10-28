#pragma once
#include "CState.h"

BEGIN(Client)
class CHit_Fall
	: public CState
{
public:
	enum HIT_TYPE { BACK, FRONT, HIT_END };

	DECLARE_STATE(CHit_Fall);
private:
	CHit_Fall();
	virtual ~CHit_Fall();

public:
	static CHit_Fall* Create();

public:
	void	On_HitFall(HIT_TYPE eType, _float4 vLook, _float4 vDir, _float fSpeed, _float fGravityPower, _float fDamage);

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	HIT_TYPE	m_eHitType = HIT_END;
	_float4 m_vDir;
	_float4 m_vLook;
	_float m_fSpeed = 0.f;
	_float m_fGravityPower = 0.f;
	_float	m_fDamage = 0.f;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
};

END