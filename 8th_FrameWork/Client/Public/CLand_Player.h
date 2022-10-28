#pragma once
#include "CState.h"

BEGIN(Client)
class CLand_Player
	: public CState
{
	DECLARE_STATE(CLand_Player);
private:
	CLand_Player();
	virtual ~CLand_Player();

public:
	static CLand_Player* Create();

public:
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

	virtual void		OnExecute(CUnit* pOwner, CAnimator* pAnimator) { m_iAnimIndex = 37; }
};

END