#pragma once
#include "CState.h"

BEGIN(Client)
class CDeath_Enemy
	: public CState
{
public:
	enum HIT_TYPE { LEFT, RIGHT, HEAD, UPHEAD, UPSHOULDER, HIT_END };

	DECLARE_STATE(CDeath_Enemy);
private:
	CDeath_Enemy();
	virtual ~CDeath_Enemy();

public:
	static CDeath_Enemy* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual STATE_TYPE Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	_bool		m_bBGM = false;
	_bool		m_bBoss = false;


private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
};

END