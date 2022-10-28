#pragma once
#include "CEffect.h"

BEGIN(Client)

class CWoodHand final
	: public CEffect
{
	DECLARE_PROTOTYPE(CWoodHand);

private:
	CWoodHand();
	virtual ~CWoodHand();

public:
	static CWoodHand* Create();

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Start() override;

public:
	virtual void My_LateTick() override;

public:
	virtual void OnEnable();
};

END