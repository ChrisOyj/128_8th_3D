#pragma once
#include "CState_Attack.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CBossChidori
	: public CState
{
	DECLARE_STATE(CBossChidori);
private:
	CBossChidori();
	virtual ~CBossChidori();

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
		CHARGESTART,
		CHARGELOOP,
		THROW,
		DASH,
		ATTACK,
		PHASE_END
	};

public:
	static CBossChidori* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	list<CGameObject*>	m_EffectsList;

	list<CGameObject*>	m_KunaiList;

private:
	_float	m_fChidoriLoopAcc = 0.f;
	_float	m_fChidoriLoopTime = 0.2f;
	_float	m_fDashTime = 0.1f;

	BOSSPHASE	m_arrBossPhase[PHASE_END];
	PHASE		m_eCurPhase = BACKDASH;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void OnExecute(CUnit* pOwner, CAnimator* pAnimator) override;


	// CState을(를) 통해 상속됨
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;



};

END