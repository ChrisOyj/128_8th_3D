#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
END

BEGIN(Client)

class CEmpty_GameObject
	: public CGameObject
{
	DECLARE_PROTOTYPE(CEmpty_GameObject);
	DECLARE_GAMEOBJECT(CEmpty_GameObject);

protected:
	CEmpty_GameObject();
	virtual ~CEmpty_GameObject();

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;
};

END