#pragma once
#include "CState.h"

BEGIN(Client)
class CWireDash_Player
	: public CState
{
	DECLARE_STATE(CWireDash_Player);
private:
	CWireDash_Player();
	virtual ~CWireDash_Player();

public:
	static CWireDash_Player* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	_uint	m_iCnt = 0;


private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual void		OnExecute(CUnit* pOwner, CAnimator* pAnimator);
	virtual void		TickExecute(CUnit* pOwner, CAnimator* pAnimator);
};

END