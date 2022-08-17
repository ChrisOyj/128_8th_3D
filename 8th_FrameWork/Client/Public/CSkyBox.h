#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Client)
class CSkyBox
	: public CGameObject
{
	DECLARE_PROTOTYPE(CSkyBox);
	DECLARE_GAMEOBJECT(CSkyBox);

private:
	CSkyBox();
	virtual ~CSkyBox();

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;

};
END
