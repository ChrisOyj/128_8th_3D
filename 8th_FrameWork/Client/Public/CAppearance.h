#pragma once
#include "CState.h"

BEGIN(Client)
class CAppearance
	: public CState
{
	DECLARE_STATE(CAppearance);

private:
	CAppearance();
	virtual ~CAppearance();

public:
	static CAppearance* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual STATE_TYPE Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void		OnExecute(CUnit* pOwner, CAnimator* pAnimator) override;
};

END