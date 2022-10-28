#pragma once
#include "Client_Defines.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)

class CUnit;

class CState
{
protected:
	CState();
	virtual ~CState();

	friend class CState_Manager;
	friend class CUnit;

public:
	virtual CState* Clone() PURE;

public:
	_bool	Is_CoolTimeOn() { return m_bCoolTime; }
	void	ReSet_CoolTime() {
		m_bCoolTime = true;
	};
	void	Set_AnimType(ANIM_TYPE eAnimType) { m_eAnimType = eAnimType; }
	void	Set_AnimIndex(_uint iAnimIndex) { m_iAnimIndex = iAnimIndex; }

public:
	virtual HRESULT		Initialize()	PURE;
	virtual void		Enter(CUnit* pOwner, CAnimator* pAnimator);
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void		Exit(CUnit* pOwner, CAnimator* pAnimator) PURE;

protected:
	vector<STATE_TYPE>		m_vecAdjState;
	STATE_TYPE			m_eStateType = STATE_END;

	ANIM_TYPE			m_eAnimType = ANIM_BASE;
	_uint				m_iAnimIndex = 0;

	_float				m_fAnimSpeed = 1.f;
	_float				m_fInterPolationTime = 0.1f;
	_float				m_fTimeAcc = 0.f;
	_float				m_fStateChangableTime = 0.f;
	_float				m_fDelayTime = 0.f;

	_bool				m_bExecuted = false;
	_bool				m_bCoolTime = true;

protected:
	CHANNEL_CLIENT	m_eChannel = CHANNEL_EFFECTS;
	wstring	m_wstrSoundKey;
	_float	m_fLoopSoundAcc = 0.f;
	_float	m_fSoundLoopTime = 0.1f;


public:
	virtual STATE_TYPE		Check_Condition(CUnit* pOwner, CAnimator* pAnimator) PURE;
	virtual void		OnExecute(CUnit* pOwner, CAnimator* pAnimator) {};
	virtual void		TickExecute(CUnit* pOwner, CAnimator* pAnimator) {};

	
};

END