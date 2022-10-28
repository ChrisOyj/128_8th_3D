#pragma once
#include "CState.h"

BEGIN(Client)
class CDomedWall_End
	: public CState
{
	DECLARE_STATE(CDomedWall_End);

private:
	CDomedWall_End();
	virtual ~CDomedWall_End();

public:
	static CDomedWall_End* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
};

END