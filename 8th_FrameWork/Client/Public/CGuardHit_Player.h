#pragma once
#include "CState.h"

BEGIN(Client)
class CGuardHit_Player
	: public CState
{
	DECLARE_STATE(CGuardHit_Player);
public:
	enum TYPE{DEFAULT, GUARDBREAK, GUARD_END};

private:
	CGuardHit_Player();
	virtual ~CGuardHit_Player();

public:
	static CGuardHit_Player* Create();

public:
	void	On_GuardHit(_float4 vLook, TYPE eType);

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual STATE_TYPE Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

public:
	_float4	m_vLook;
	TYPE	m_eType = GUARD_END;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
};

END