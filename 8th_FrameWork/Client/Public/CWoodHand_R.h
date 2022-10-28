#pragma once
#include "CEffect.h"

BEGIN(Client)

class CWoodHand_R final
	: public CEffect
{
	DECLARE_PROTOTYPE(CWoodHand_R);

private:
	CWoodHand_R();
	virtual ~CWoodHand_R();

public:
	static CWoodHand_R* Create();

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