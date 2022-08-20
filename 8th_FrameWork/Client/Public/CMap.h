#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Client)
class CMap
	: public CGameObject
{
	DECLARE_PROTOTYPE(CMap);

private:
	CMap();
	virtual ~CMap();

public:
	static CMap* Create(const char* pFilePath);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;

private:
	HRESULT		SetUp_Model(const char* pFilePath);

};
END
