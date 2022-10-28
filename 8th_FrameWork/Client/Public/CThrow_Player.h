#pragma once
#include "CState.h"

BEGIN(Client)
class CThrow_Player
	: public CState
{
	DECLARE_STATE(CThrow_Player);
private:
	CThrow_Player();
	virtual ~CThrow_Player();

public:
	static CThrow_Player* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	class CWindmill* m_pWindmill = nullptr;


private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void OnExecute(CUnit* pOwner, CAnimator* pAnimator) override;
};

END