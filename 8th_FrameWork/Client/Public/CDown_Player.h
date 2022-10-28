#pragma once
#include "CState.h"

BEGIN(Client)
class CDown_Player
	: public CState
{
	DECLARE_STATE(CDown_Player);
private:
	CDown_Player();
	virtual ~CDown_Player();

public:
	static CDown_Player* Create();

public:
	void	Set_Front(_bool bFront) { m_bFront = bFront; }

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	_bool	m_bFront = false;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
};

END