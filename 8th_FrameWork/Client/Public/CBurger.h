#pragma once
#include "CEffect.h"

BEGIN(Client)

class CBurger final
	: public CEffect
{
	DECLARE_PROTOTYPE(CBurger);


private:
	CBurger();
	virtual ~CBurger();

public:
	static CBurger* Create();

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Start() override;

private:
	_bool		m_bEnter = false;

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;

private:
	virtual void OnEnable();
};

END