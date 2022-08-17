#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Client)
class CTerrain
	: public CGameObject
{
	DECLARE_PROTOTYPE(CTerrain);
	DECLARE_GAMEOBJECT(CTerrain);

private:
	CTerrain();
	virtual ~CTerrain();

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;

};
END
