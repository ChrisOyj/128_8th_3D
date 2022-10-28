#pragma once
#include "CEffect.h"

BEGIN(Engine)
END

BEGIN(Client)
class CDefault_Effect
	: public CEffect
{
public:

	DECLARE_PROTOTYPE(CDefault_Effect)

private:
	CDefault_Effect();
	virtual ~CDefault_Effect();

public:
	static CDefault_Effect* Create();
	static CDefault_Effect* Create(ifstream* pReadFile);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;

private:
	HRESULT	SetUp_DefaultEffect(ifstream* pReadFile);

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;



};

END