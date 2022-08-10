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
	COM_MESH,
	COM_FADER,
	COM_END
};

class CComponent_Factory
{
public:
	static	HRESULT		Save_Json(const _uint& iID, CComponent* pComponent);

public:
	static CComponent* Create_FromJson(const _uint& iID, CGameObject* pOwner);
	static CComponent* Create_FromPrototype(const _uint& iID, CGameObject* pOwner);

private:
	static CComponent* Create_PrototypeFromJson(const _uint& iComponentID);
	static CComponent* Create_InstanceFromJson(const json& _json);

private:
	static void			Save_Transform(CComponent* pComponent, json* pOut);
	static void			Save_Collider(CComponent* pComponent, json* pOut);
	static void			Save_Renderer(CComponent* pComponent, json* pOut);
	static void			Save_Physics(CComponent* pComponent, json* pOut);
	static void			Save_Texture(CComponent* pComponent, json* pOut);
	static void			Save_Shader(CComponent* pComponent, json* pOut);
	static void			Save_Fader(CComponent* pComponent, json* pOut);
	static void			Save_Mesh(CComponent* pComponent, json* pOut);
};

END