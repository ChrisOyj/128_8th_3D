#pragma once
#include "CState.h"

BEGIN(Client)
class CIdle_NightGuy
	: public CState
{
	DECLARE_STATE(CIdle_NightGuy);

private:
	CIdle_NightGuy();
	virtual ~CIdle_NightGuy();

public:
	static CIdle_NightGuy* Create();

public:
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
};

END