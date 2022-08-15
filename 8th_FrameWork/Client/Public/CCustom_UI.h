#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
END

BEGIN(Client)

class CCustom_UI abstract
	: public CGameObject
{
protected:
	CCustom_UI();
	virtual ~CCustom_UI();

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;
};

END