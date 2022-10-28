#pragma once
#include "CState.h"

BEGIN(Client)
class CGoTarget
	: public CState
{
	DECLARE_STATE(CGoTarget);
private:
	CGoTarget();
	virtual ~CGoTarget();

public:
	static CGoTarget* Create();

public:
	void		Set_TargetPos(_float4 vTargetPos) { m_vTargetPos = vTargetPos; }

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	_float4	m_vTargetPos;


private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
};

END