#pragma once
#include "CState.h"

BEGIN(Client)
class CFall_Go
	: public CState
{
	DECLARE_STATE(CFall_Go);
private:
	CFall_Go();
	virtual ~CFall_Go();

public:
	static CFall_Go* Create();

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