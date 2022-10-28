#pragma once
#include "CState.h"

BEGIN(Client)
class CDoubleJump_Player
	: public CState
{
	DECLARE_STATE(CDoubleJump_Player);
private:
	CDoubleJump_Player();
	virtual ~CDoubleJump_Player();

public:
	static CDoubleJump_Player* Create();

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