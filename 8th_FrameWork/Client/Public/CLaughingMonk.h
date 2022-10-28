#pragma once
#include "CState_Attack_Enemy.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CLaughingMonk
	: public CState_Attack_Enemy
{
	DECLARE_STATE(CLaughingMonk);
private:
	CLaughingMonk();
	virtual ~CLaughingMonk();

public:
	static CLaughingMonk* Create();

public:
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void OnExecute(CUnit* pOwner, CAnimator* pAnimator) override;


	// CState��(��) ���� ��ӵ�
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;



};

END