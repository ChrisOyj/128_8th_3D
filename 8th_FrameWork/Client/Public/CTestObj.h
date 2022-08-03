#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Client)

class CTestObj final
	: public CGameObject
{
	DECLARE_PROTOTYPE(CTestObj);
	DECLARE_GAMEOBJECT(CTestObj);

private:
	CTestObj();
	virtual ~CTestObj();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;

};

END