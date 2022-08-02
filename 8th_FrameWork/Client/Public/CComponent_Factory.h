#pragma once
#include "Client_Defines.h"

BEGIN(Engine)
class CTransform;
class CComponent;
class CGameObject;
END

BEGIN(Client)

enum COMPONENT_TYPE
{
	COM_TRANSFORM,
	COM_COLLIDER,
	COM_RENDERER,
	COM_PHYSICS,
	COM_TEXTURE,
	COM_SHADER,
	COM_END
};

class CComponent_Factory
{
public:
	static CComponent* Create_FromJson(const _uint& iID, CGameObject* pOwner);
	static CComponent* Create_FromPrototype(const _uint& iID, CGameObject* pOwner);

private:
	static CComponent* Create_PrototypeFromJson(const _uint& iComponentID);
	static CComponent* Create_InstanceFromJson(const json& _json);
};

END