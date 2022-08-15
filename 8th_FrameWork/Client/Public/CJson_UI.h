#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
END

BEGIN(Client)

class CJson_UI final
	: public CGameObject
{
	DECLARE_PROTOTYPE(CJson_UI);

private:
	CJson_UI();
	virtual ~CJson_UI();

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;
};

END