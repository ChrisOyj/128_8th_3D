#pragma once
#include "CState.h"

BEGIN(Client)
class CDodge_Player
	: public CState
{
	DECLARE_STATE(CDodge_Player);
private:
	CDodge_Player();
	virtual ~CDodge_Player();

public:
	static CDodge_Player* Create();

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