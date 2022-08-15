#pragma once
#include "Client_Defines.h"

BEGIN(Client)

class CPrototype_Factory
{

public:
	static HRESULT	SetUp_DefaultComponents();
	static HRESULT	SetUp_DefaultGameObjects();
};
END