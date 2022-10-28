#pragma once
#include "CState.h"

BEGIN(Client)
class CGetUp_Enemy
	: public CState
{
	DECLARE_STATE(CGetUp_Enemy);
private:
	CGetUp_Enemy();
	virtual ~CGetUp_Enemy();

public:
	static CGetUp_Enemy* Create();

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