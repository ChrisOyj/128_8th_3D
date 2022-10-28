#pragma once
#include "CState_Attack.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CNightGuy_Run
	: public CState_Attack
{
	DECLARE_STATE(CNightGuy_Run);
private:
	CNightGuy_Run();
	virtual ~CNightGuy_Run();

public:
	static CNightGuy_Run* Create();

public:
	virtual void	OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos);

public:
	void	Set_Dragon(CGameObject* pDragon) { m_EffectsList.push_back(pDragon); }

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