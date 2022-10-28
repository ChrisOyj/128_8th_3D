#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Client)
class CGround
	: public CGameObject
{
	DECLARE_PROTOTYPE(CGround);

private:
	CGround();
	virtual ~CGround();

public:
	static CGround* Create(wstring wstrFilePath);


public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT	Start() override;

	virtual void My_LateTick() override;

private:
	HRESULT		SetUp_Model(wstring wstrFilePath);

};
END
