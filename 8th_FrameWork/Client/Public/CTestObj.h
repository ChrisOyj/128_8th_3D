#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Client)
class CTestObj
	: public CGameObject
{
	DECLARE_PROTOTYPE(CTestObj);
	DECLARE_GAMEOBJECT(CTestObj);

private:
	CTestObj();
	virtual ~CTestObj();

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;

};
END
