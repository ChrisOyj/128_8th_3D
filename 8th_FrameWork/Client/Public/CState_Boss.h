#pragma once
#include "CState.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CState_Boss
	: public CState
{
private:
	struct BOSSPHASE
	{
		_float	fDuration = 0.f;
		_uint	iAnimIndex = 0;
		_uint	iAnimType = 0;
		_float	fAnimSpeed = 1.f;
		_float	fInterPolationTime = 0.f;
	};

	enum PHASE
	{
		CRYING,
		CHARGESTART,
		CHARGELOOP,
		CHARGEEND,
		FALL,
		LAND,
		ANGRY,
		PHASE_END
	};
	DECLARE_STATE(CState_Boss);
private:
	CState_Boss();
	virtual ~CState_Boss();

	friend class CWindow_Boss;

public:
	static CState_Boss* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	_bool		m_bFirst = false;
	_float4		m_vOriginPos;
	BOSSPHASE	m_arrBossPhase[PHASE_END];
	PHASE		m_eCurPhase = CRYING;
	_float		m_fJumpPower = 30.f;
	_float		m_fSpeed = 6.f;

	_bool	m_bOnce = false;
	CGameObject* m_pChargeEffect = nullptr;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

	virtual void		OnExecute(CUnit* pOwner, CAnimator* pAnimator) { m_iAnimIndex = 37; }
};

END