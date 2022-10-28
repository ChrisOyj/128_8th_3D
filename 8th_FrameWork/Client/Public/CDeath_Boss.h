#pragma once
#include "CState.h"

BEGIN(Client)
class CDeath_Boss
	: public CState
{
public:
	enum HIT_TYPE { LEFT, RIGHT, HEAD, UPHEAD, UPSHOULDER, HIT_END };

	DECLARE_STATE(CDeath_Boss);
private:
	CDeath_Boss();
	virtual ~CDeath_Boss();

public:
	static CDeath_Boss* Create();

public:
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual STATE_TYPE Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;


private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
};

END