#pragma once
#include "CState.h"

BEGIN(Client)
class CDodge_Enemy
	: public CState
{
	DECLARE_STATE(CDodge_Enemy);
private:
	CDodge_Enemy();
	virtual ~CDodge_Enemy();

public:
	static CDodge_Enemy* Create();

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