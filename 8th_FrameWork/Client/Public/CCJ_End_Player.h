#pragma once
#include "CState.h"

BEGIN(Client)
class CCJ_End_Player
	: public CState
{
	DECLARE_STATE(CCJ_End_Player);
private:
	CCJ_End_Player();
	virtual ~CCJ_End_Player();

public:
	static CCJ_End_Player* Create();

public:
	void		Set_ChargeTime(_float fChargeTime) { m_fChargeTime = fChargeTime; }

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	_float m_fChargeTime = 0.f;
private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
};

END