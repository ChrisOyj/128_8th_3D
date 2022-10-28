#pragma once
#include "CState_Attack.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CBossFire
	: public CState
{
	DECLARE_STATE(CBossFire);
private:
	CBossFire();
	virtual ~CBossFire();

	struct BOSSPHASE
	{
		_float	fDuration = 0.f;
		_uint	iAnimIndex = 0;
		_uint	iAnimType = 0;
		_float	fAnimSpeed = 1.f;
		_float	fInterPolationTime = 0.f;
		_float	fDelayTime = 0.f;
	};

	enum PHASE
	{
		BACKDASH,
		JUMP,
		FIRE,
		PHASE_END
	};

public:
	static CBossFire* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	BOSSPHASE	m_arrBossPhase[PHASE_END];
	PHASE		m_eCurPhase = BACKDASH;

	_uint		m_iCurFireCount = 0;
	_uint		m_iMaxFireCount = 3;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void OnExecute(CUnit* pOwner, CAnimator* pAnimator) override;


	// CState을(를) 통해 상속됨
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;



};

END