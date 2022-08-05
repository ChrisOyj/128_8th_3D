#pragma once
#include "Client_Defines.h"

BEGIN(Client)

class CPrototype_Factory
{
public:
	enum COMPONETS_PROTOTYPE_ID 
	{
		DEFAULT_SHADER = 120000,
		DEFAULT_RENDERER,
		DEFAULT_MESH_RECT,
		DEFAULT_UI_RENDERER,
		COMPONETS_PROTOTYPE_ID_END
	};

public:
	static HRESULT	SetUp_DefaultComponents();
};
END