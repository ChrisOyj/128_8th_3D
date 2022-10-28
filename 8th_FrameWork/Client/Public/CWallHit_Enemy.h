#pragma once
#include "CState.h"

BEGIN(Client)
class CWallHit_Enemy
	: public CState
{
	DECLARE_STATE(CWallHit_Enemy);
private:
	CWallHit_Enemy();
	virtual ~CWallHit_Enemy();

public:
	static CWallHit_Enemy* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;


private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
};

END