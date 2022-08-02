#pragma once
#include "Camera.h"
#include "Client_Defines.h"

BEGIN(Client)

class CCamera_Free final
	: public CCamera
{
	DECLARE_PROTOTYPE(CCamera_Free);
	DECLARE_GAMEOBJECT(CCamera_Free);

private:
	CCamera_Free();
	virtual ~CCamera_Free();


public:
	// CCamera을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;

private:

};

END