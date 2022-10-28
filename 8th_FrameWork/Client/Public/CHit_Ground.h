#pragma once
#include "CState.h"

BEGIN(Client)
class CHit_Ground
	: public CState
{
	DECLARE_STATE(CHit_Ground);
public:
	enum HIT_TYPE {LEFT, RIGHT, HEAD, UPHEAD, UPSHOULDER, CHIDORIHIT, HARDHIT, SPINBLOW, HIT_END};

private:
	CHit_Ground();
	virtual ~CHit_Ground();

public:
	static CHit_Ground* Create();

public:
	void	On_HitGround(HIT_TYPE eType, _float4 vLook, _float4 vDir, _float fSpeed, _float fJumpPower, _float fDamage);
	void	On_HitGround(HIT_TYPE eType, _float fSpeed, _float fJumpPower, _float fDamage);
	void	On_HitGround(_float4 vLook, _float4 vDir);
	void	On_HitGroundDefault(_float4 vLook, _float4 vDir);

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual STATE_TYPE Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	HIT_TYPE	m_eHitType = HIT_END;
	_float4 m_vDir;
	_float4 m_vLook;
	_float m_fSpeed = 0.f;
	_float m_fJumpPower = 0.f;
	_float	m_fDamage = 0.f;


private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
};

END