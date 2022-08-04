#pragma once
#include "Client_Defines.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CUtility_Transform
{
public:
	static	void	Turn_ByAngle(CTransform* pTransform, _float4 vAxis, _float fAngle);
};
END
