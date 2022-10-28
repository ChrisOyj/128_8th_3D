#pragma once
#include "CEffect.h"

BEGIN(Engine)
class CHierarchyNode;
END

BEGIN(Client)

class CUnit;

class CWireKunai
	: public CEffect
{
	DECLARE_PROTOTYPE(CWireKunai);

private:
	CWireKunai();
	virtual ~CWireKunai();

public:
	static CWireKunai* Create();

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Start() override;

private:
	_bool		m_bSound = false;

private:
	virtual void OnEnable() override;
	virtual void My_Tick() override;

};

END