#pragma once
#include "Client_Defines.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CUtility_FBX
{
public:
	static	CModel* Load_FBX(Engine::MODEL_TYPE eModelType, const char* pModelFilePath, _float4x4 TransformMatrix);
};

END