#pragma once
#include "CState_Attack.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CChidori_Run
	: public CState
{
	DECLARE_STATE(CChidori_Run);
private:
	CChidori_Run();
	virtual ~CChidori_Run();

public:
	static CChidori_Run* Create();

public:
	virtual void	OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos);


public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	list<CGameObject*>	m_EffectsList;

private:
	_float	m_fChidoriLoopAcc = 0.f;
	_float	m_fChidoriLoopTime = 0.2f;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void OnExecute(CUnit* pOwner, CAnimator* pAnimator) override;


	// CState을(를) 통해 상속됨
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

};

END