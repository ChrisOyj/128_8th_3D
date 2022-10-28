#pragma once
#include "CState_Attack.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CLeafHurricane_Enemy
	: public CState
{
	DECLARE_STATE(CLeafHurricane_Enemy);
private:
	CLeafHurricane_Enemy();
	virtual ~CLeafHurricane_Enemy();

public:
	static CLeafHurricane_Enemy* Create();

public:
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;


	// CState��(��) ���� ��ӵ�
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

};

END