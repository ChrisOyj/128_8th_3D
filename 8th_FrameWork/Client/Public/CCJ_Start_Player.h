#pragma once
#include "CState.h"

BEGIN(Client)
class CCJ_Start_Player
	: public CState
{
	DECLARE_STATE(CCJ_Start_Player);
private:
	CCJ_Start_Player();
	virtual ~CCJ_Start_Player();

public:
	static CCJ_Start_Player* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual STATE_TYPE Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

	virtual void OnExecute(CUnit* pOwner, CAnimator* pAnimator) override;
};

END