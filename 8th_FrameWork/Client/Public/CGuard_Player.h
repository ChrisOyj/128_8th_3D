#pragma once
#include "CState.h"

BEGIN(Client)
class CGuard_Player
	: public CState
{
	DECLARE_STATE(CGuard_Player);
private:
	CGuard_Player();
	virtual ~CGuard_Player();

public:
	static CGuard_Player* Create();

public:
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual STATE_TYPE Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
};

END