#pragma once
#include "CState.h"

BEGIN(Client)
class CWallJump_NightGuy
	: public CState
{
	DECLARE_STATE(CWallJump_NightGuy);
private:
	CWallJump_NightGuy();
	virtual ~CWallJump_NightGuy();

public:
	static CWallJump_NightGuy* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	_float m_fTargetY = 0.f;
	_float	m_fEffectTimeAcc = 0.3f;
	_float m_fEffectTime = 0.2f;
	_float	m_fEffectDegree = 30.f;
	_float4 m_vEffectUp = _float4(-1.f, 1.5f, 0.f, 0.f);
	_float4	m_vOffsetPos = _float4(0.8f, 1.f, -0.5f, 1.f);

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
};

END