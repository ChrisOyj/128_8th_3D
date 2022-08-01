#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CScriptable abstract
	: public CComponent
{
protected:
	CScriptable(_uint iGroupID);
	virtual ~CScriptable();
};
END
