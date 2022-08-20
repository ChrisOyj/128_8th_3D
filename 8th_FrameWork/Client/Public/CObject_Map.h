#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Client)
class CObject_Map
	: public CGameObject
{
	DECLARE_PROTOTYPE(CObject_Map);

private:
	CObject_Map();
	virtual ~CObject_Map();

public:
	static CObject_Map* Create(const char* pDirectoryPath);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;

private:
	HRESULT		SetUp_ObjectsMap(const char* pFilePath);

};
END
