#pragma once
#include "CState.h"

BEGIN(Client)
class CAirDash_Player
	: public CState
{
	DECLARE_STATE(CAirDash_Player);
private:
	CAirDash_Player();
	virtual ~CAirDash_Player();

public:
	static CAirDash_Player* Create();

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