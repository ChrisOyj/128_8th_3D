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
	static CComponent* Create(const _uint& iID, CGameObject* pOwner);

	static CTransform* Create_Transform(const _uint& iID);

private:
	static CComponent* Create_PrototypeFromJson(const _uint& iComponentID, CGameObject* pOwner);
	static CComponent* Create_InstanceFromJson(const json& _json, CGameObject* pOwner);
};

END