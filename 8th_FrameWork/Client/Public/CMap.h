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
	static CMap* Create(wstring wstrFilePath);

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

private:
	HRESULT		SetUp_Model(wstring wstrFilePath);

};
END
