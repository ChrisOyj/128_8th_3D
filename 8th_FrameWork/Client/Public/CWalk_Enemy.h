#pragma once
#include "CState.h"

BEGIN(Client)
class CWalk_Enemy
	: public CState
{
	DECLARE_STATE(CWalk_Enemy);
private:
	CWalk_Enemy();
	virtual ~CWalk_Enemy();

public:
	static CWalk_Enemy* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	_float		m_fSign = 1.f;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
};

END