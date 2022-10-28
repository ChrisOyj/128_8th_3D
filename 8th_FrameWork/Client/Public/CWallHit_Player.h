#pragma once
#include "CState.h"

BEGIN(Client)
class CWallHit_Player
	: public CState
{
	DECLARE_STATE(CWallHit_Player);
private:
	CWallHit_Player();
	virtual ~CWallHit_Player();

public:
	static CWallHit_Player* Create();

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