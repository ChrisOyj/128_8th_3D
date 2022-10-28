#pragma once
#include "CState.h"

BEGIN(Client)
class CFall_NightGuy
	: public CState
{
	DECLARE_STATE(CFall_NightGuy);
private:
	CFall_NightGuy();
	virtual ~CFall_NightGuy();

public:
	static CFall_NightGuy* Create();

public:
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
};

END