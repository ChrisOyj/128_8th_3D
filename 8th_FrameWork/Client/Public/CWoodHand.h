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
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Start() override;

public:
	virtual void My_LateTick() override;

public:
	virtual void OnEnable();
};

END