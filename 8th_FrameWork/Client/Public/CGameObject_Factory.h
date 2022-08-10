#pragma once
#include "Client_Defines.h"

BEGIN(Engine)
class CTransform;
class CGameObject;
END

BEGIN(Client)

enum GAMEOBJECT_TYPE
{
	OBJ_UNIT,
	OBJ_UI,
	OBJ_EFFECT,
	OBJ_END
};

class CGameObject_Factory
{
public:
	static CGameObject* Create_FromPrototype(const _uint& iID);
	static CGameObject* Create_FromJson(const _uint& iID);

private:
	static CGameObject* Create_PrototypeFromJson(const _uint& iGameObjectID);
	static CGameObject* Create_InstanceFromJson(const _uint& iGameObjectID, const json& _json);

	static HRESULT Add_ComponentsToGameObject(CGameObject* pGameObject, const json& _ComponentListjson);
	static HRESULT Add_ChildrenObjectsToGameObject(CGameObject* pGameObject, const json& _ChildrenListjson);
};

END