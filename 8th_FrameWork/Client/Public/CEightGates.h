#pragma once
#include "CState_Attack.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CEightGates
	: public CState
{
	DECLARE_STATE(CEightGates);
private:
	CEightGates();
	virtual ~CEightGates();

public:
	static CEightGates* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	_uint	m_iStateChangableFrame = 0;
	_uint	m_iFirstExecuteFrame = 0;
	_uint	m_iSecondExecuteFrame = 0;

	_bool	m_bFirstExecuted = false;

	CGameObject* m_pRasenShuriken = nullptr;

	_uint		m_iOriginEyeIndex = 0;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	void FirstExecute(CUnit* pOwner, CAnimator* pAnimator);
	virtual void OnExecute(CUnit* pOwner, CAnimator* pAnimator) override;


	// CState을(를) 통해 상속됨
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;



};

END