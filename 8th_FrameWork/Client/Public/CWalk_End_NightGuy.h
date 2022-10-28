#pragma once
#include "CState.h"

BEGIN(Client)
class CWalk_End_NightGuy
	: public CState
{
	DECLARE_STATE(CWalk_End_NightGuy);

private:
	CWalk_End_NightGuy();
	virtual ~CWalk_End_NightGuy();

public:
	static CWalk_End_NightGuy* Create();

public:
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
};

END