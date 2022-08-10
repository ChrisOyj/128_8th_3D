#include "stdafx.h"
#include "CGameObject_Factory.h"

#include "CComponent_Factory.h"

#include "GameInstance.h"

#include "Transform.h"
#include "CCollider_Sphere.h"

#include "CUtility_Json.h"

#include "GameObject.h"

#include "CUI.h"
#include "CEmpty_GameObject.h"

#define JSON_GAMEOBJECT_TYPE	"GameObject_Type"
#define JSON_GAMEOBJECT_COMPONENT_LIST	"Component_List"
#define JSON_GAMEOBJECT_CHILDREN_LIST	"Children_List"

CGameObject* CGameObject_Factory::Create_FromJson(const _uint& iID)
{
	CGameObject* pGameObject = CLONE_GAMEOBJECT(iID);

	if (!pGameObject)
	{
		pGameObject = Create_PrototypeFromJson(iID);
	}

	if (FAILED(pGameObject->Initialize()))
	{
		Call_MsgBox_Index(L"Failed to Initialize : Index ->", iID);
		return nullptr;
	}

	return pGameObject;
}

CGameObject* CGameObject_Factory::Create_FromPrototype(const _uint& iID)
{
	CGameObject* pGameObject = CLONE_GAMEOBJECT(iID);

	if (!pGameObject)
		return nullptr;

	if (FAILED(pGameObject->Initialize()))
	{
		Call_MsgBox_Index(L"Failed to Initialize : Index ->", iID);
		return nullptr;
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

	json GameObjectJson;
	
	if (FAILED(CUtility_Json::Load_Json(CUtility_Json::Complete_Path(iGameObjectID).c_str(), &GameObjectJson)))
	{
		Call_MsgBox_Index(L"Failed to Load_Json : CGameObject_Factory", iGameObjectID);
		return nullptr;
	}

	pGameObject = Create_InstanceFromJson(iGameObjectID, GameObjectJson);

	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject_Prototype(iGameObjectID, pGameObject)))
		return nullptr;


	return pGameObject->Clone();
}

CGameObject* CGameObject_Factory::Create_InstanceFromJson(const _uint& iGameObjectID, const json& _json)
{
	GAMEOBJECT_TYPE	eGameObjectType = _json[JSON_GAMEOBJECT_TYPE];
	CGameObject* pGameObject = nullptr;

	switch (eGameObjectType)
	{
	case Client::OBJ_UNIT:
		break;

	case Client::OBJ_UI:
		pGameObject = CEmpty_GameObject::Create();
		pGameObject->Set_ID(iGameObjectID);
		break;

	case Client::OBJ_EFFECT:
		break;

	default:
		break;
	}

	if (FAILED(Add_ComponentsToGameObject(pGameObject, _json[JSON_GAMEOBJECT_COMPONENT_LIST])))
		return nullptr;

	auto iter = _json.find(JSON_GAMEOBJECT_CHILDREN_LIST);

	if (iter != _json.end() &&
		FAILED(Add_ChildrenObjectsToGameObject(pGameObject, _json[JSON_GAMEOBJECT_CHILDREN_LIST])))
		return nullptr;

	if (FAILED(pGameObject->Initialize_Prototype()))
	{
		Call_MsgBox_Index(L"Failed to Initialize_Prototype : CComponent_Enum ->", eGameObjectType);
		return nullptr;
	}

	pGameObject->Get_Transform()->Get_Transform().vScale = CUtility_Json::Get_VectorFromJson(_json["vScale"]);
	pGameObject->Get_Transform()->Get_Transform().matMyWorld = CUtility_Json::Get_MatrixFromJson(_json["WorldMatrix"]);

	return pGameObject;
}

HRESULT CGameObject_Factory::Add_ComponentsToGameObject(CGameObject* pGameObject, const json& _ComponentListjson)
{
	for (_uint i = 0; i < _ComponentListjson.size(); ++i)
	{
		_uint iComponentID = _ComponentListjson[i];

		if (!Safe_CheckID(iComponentID, ID_COMPONENT))
			return E_FAIL;

		CComponent* pComponent = CComponent_Factory::Create_FromJson(iComponentID, pGameObject);

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

		CGameObject* pChild = CGameObject_Factory::Create_FromJson(iGameObjectID);

		if (!pChild)
			return E_FAIL;

		pGameObject->Add_Child(pChild);
	}

	return S_OK;
}
