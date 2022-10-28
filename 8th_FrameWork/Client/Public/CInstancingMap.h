#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Client)
class CInstancingMap
	: public CGameObject
{
	DECLARE_PROTOTYPE(CInstancingMap);

private:
	CInstancingMap();
	virtual ~CInstancingMap();

public:
	static CInstancingMap* Create(wstring wstrFilePath, wstring wstrInstancingPath);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

private:
	HRESULT		SetUp_Model(wstring wstrFilePath, wstring wstrInstancingPath);

};
END
