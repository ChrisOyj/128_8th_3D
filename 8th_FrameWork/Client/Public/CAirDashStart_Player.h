#pragma once
#include "CState.h"

BEGIN(Client)
class CAirDashStart_Player
	: public CState
{
	DECLARE_STATE(CAirDashStart_Player);
private:
	CAirDashStart_Player();
	virtual ~CAirDashStart_Player();

public:
	static CAirDashStart_Player* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;


private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual void		OnExecute(CUnit* pOwner, CAnimator* pAnimator);
	virtual void		TickExecute(CUnit* pOwner, CAnimator* pAnimator);
};

END