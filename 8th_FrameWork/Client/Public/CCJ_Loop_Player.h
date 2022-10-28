#pragma once
#include "CState.h"

BEGIN(Client)
class CCJ_Loop_Player
	: public CState
{
	DECLARE_STATE(CCJ_Loop_Player);
private:
	CCJ_Loop_Player();
	virtual ~CCJ_Loop_Player();

public:
	static CCJ_Loop_Player* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	_float		m_fChargeMaxTime = 0.f;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void OnExecute(CUnit* pOwner, CAnimator* pAnimator) override;

};

END