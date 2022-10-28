#pragma once
#include "CEffect.h"

BEGIN(Client)

class CGroundBreak final
	: public CEffect
{
	DECLARE_PROTOTYPE(CGroundBreak);

private:
	CGroundBreak();
	virtual ~CGroundBreak();

public:
	static CGroundBreak* Create(wstring	wstrPath, wstring wstrHashcode);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Start() override;

public:
	virtual void OnEnable();
};

END