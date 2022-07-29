#include "stdafx.h"
#include "CGameObject_Factory.h"

#include "CComponent_Factory.h"

#include "GameInstance.h"

#include "Transform.h"
#include "CCollider_Sphere.h"

#include "CUtility_Json.h"

#include "GameObject.h"

#define JSON_GAMEOBJECT_TYPE	"GameObject_Type"
#define JSON_GAMEOBJECT_COMPONENT_LIST	"Component_List"
#define JSON_GAMEOBJECT_Children_LIST	"Children_List"

CGameObject* CGameObject_Factory::Create(const _uint& iID)
{
	CGameObject* pGameObject = CLONE_GAMEOBJECT(iID);

	if (!pGameObject)
	{
		pGameObject = Create_PrototypeFromJson(iID);
	}

	return pGameObject;
}

CGameObject* CGameObject_Factory::Create_PrototypeFromJson(const _uint& iGameObjectID)
{
	if (!Safe_CheckID(iGameObjectID, ID_GAMEOBJECT))
	{
		Call_MsgBox(L"GameObjectID Out of Range : CGameObject_Factory");
		return nullptr;
	}

	CGameObject* pGameObject = nullptr;
	// 1. 어떤 클래스인지 enum으로 받음.
	// 2. 스위치문으로 분기해서 그에 맞게 정보 넣어주고 제작

	json GameObjectJson = CUtility_Json::Load_Json(CUtility_Json::Complete_Path(iGameObjectID));

	pGameObject = Create_InstanceFromJson(GameObjectJson);

	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject_Prototype(iGameObjectID, pGameObject)))
		return nullptr;


	return pGameObject->Clone();
}

CGameObject* CGameObject_Factory::Create_InstanceFromJson(const json& _json)
{
	GAMEOBJECT_TYPE	eGameObjectType = _json[JSON_GAMEOBJECT_TYPE];
	CGameObject* pGameObject = nullptr;

	switch (eGameObjectType)
	{
	case Client::OBJ_UNIT:
		break;

	case Client::OBJ_UI:
		break;

	case Client::OBJ_EFFECT:
		break;

	default:
		break;
	}

	if (FAILED(Add_ComponentsToGameObject(pGameObject, _json[JSON_GAMEOBJECT_COMPONENT_LIST])))
		return nullptr;

	if (FAILED(Add_ChildrenObjectsToGameObject(pGameObject, _json[JSON_GAMEOBJECT_COMPONENT_LIST])))
		return nullptr;

	if (FAILED(pGameObject->Initialize()))
	{
		Call_MsgBox_Index(L"Failed to Initialize : CComponent_Enum ->", eGameObjectType);
		return nullptr;
	}


	return pGameObject;
}

HRESULT CGameObject_Factory::Add_ComponentsToGameObject(CGameObject* pGameObject, const json& _ComponentListjson)
{
	for (_uint i = 0; i < _ComponentListjson.size(); ++i)
	{
		_uint iComponentID = _ComponentListjson[i];

		if (!Safe_CheckID(iComponentID, ID_COMPONENT))
			return E_FAIL;

		CComponent* pComponent = CComponent_Factory::Create(iComponentID, pGameObject);

		if (!pComponent)
			return E_FAIL;

		pGameObject->Add_Component(pComponent);
	}


	return S_OK;
}

HRESULT CGameObject_Factory::Add_ChildrenObjectsToGameObject(CGameObject* pGameObject, const json& _ChildrenListjson)
{
	for (_uint i = 0; i < _ChildrenListjson.size(); ++i)
	{
		_uint iGameObjectID = _ChildrenListjson[i];

		if (!Safe_CheckID(iGameObjectID, ID_GAMEOBJECT))
			return E_FAIL;

		CGameObject* pChild = CGameObject_Factory::Create(iGameObjectID);

		if (!pChild)
			return E_FAIL;

		pGameObject->Add_Child(pChild);
	}

	return S_OK;
}
