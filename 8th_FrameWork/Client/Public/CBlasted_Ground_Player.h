#pragma once
#include "CState.h"

BEGIN(Client)
class CBlasted_Ground_Player
	: public CState
{

	DECLARE_STATE(CBlasted_Ground_Player);
private:
	CBlasted_Ground_Player();
	virtual ~CBlasted_Ground_Player();

public:
	static CBlasted_Ground_Player* Create();

public:
	void	On_BlastedGround(_float4 vLook, _float4 vDir, _float fSpeed, _float fJumpPower, _float fDamage);

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual STATE_TYPE Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	_float	m_fJumpPower = 0.f;
	_float4 m_vDir;
	_float4 m_vLook;
	_float m_fSpeed = 0.f;
	_float m_fDamage = 0.f;
private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
};

END