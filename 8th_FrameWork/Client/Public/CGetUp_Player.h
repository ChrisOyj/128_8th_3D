#pragma once
#include "CState.h"

BEGIN(Client)
class CGetUp_Player
	: public CState
{
	DECLARE_STATE(CGetUp_Player);
private:
	CGetUp_Player();
	virtual ~CGetUp_Player();

public:
	static CGetUp_Player* Create();

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