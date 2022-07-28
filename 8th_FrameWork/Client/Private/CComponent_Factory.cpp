#include "stdafx.h"
#include "CComponent_Factory.h"

#include "GameInstance.h"

#include "Transform.h"
#include "CCollider_Sphere.h"

#include "CUtility_Json.h"
#include "GameObject.h"


#define JSON_COMPONENT_TYPE	"Component_Type"
#define JSON_COLLIDER_INFO	"Collider_Info"

CComponent* CComponent_Factory::Create(const _uint& iID, CGameObject* pOwner)
{
	CComponent* pComponent = CLONE_COMPONENT(iID);

	if (!pComponent)
	{
		pComponent = Create_PrototypeFromJson(iID, pOwner);
	}

	return pComponent;
}

CTransform* CComponent_Factory::Create_Transform(const _uint& iID)
{
	CComponent* pComponent = CLONE_COMPONENT(iID);

	if (!pComponent)
		return nullptr;

	CTransform* pTransform = dynamic_cast<CTransform*>(pComponent);

	return pTransform;
}

CComponent* CComponent_Factory::Create_PrototypeFromJson(const _uint& iComponentID, CGameObject* pOwner)
{
	if (!Safe_CheckID(iComponentID, ID_COMPONENT))
	{
		Call_MsgBox(L"ComponentID Out of Range : CComponent_Factory");
		return nullptr;
	}

	CComponent* pComponent = nullptr;
	// 1. 어떤 클래스인지 enum으로 받음.
	// 2. 스위치문으로 분기해서 그에 맞게 정보 넣어주고 제작

	json ComponentJson = CUtility_Json::Load_Json(CUtility_Json::Complete_Path(iComponentID));

	pComponent = Create_InstanceFromJson(ComponentJson, pOwner);

	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(iComponentID, pComponent)))
		return nullptr;


	return pComponent->Clone();
}

CComponent* CComponent_Factory::Create_InstanceFromJson(const json& _json, CGameObject* pOwner)
{
	COMPONENT_TYPE	eComponentID = _json[JSON_COMPONENT_TYPE];

	CComponent* pComponent = nullptr;

	switch (eComponentID)
	{
	case Client::COM_TRANSFORM:
		pComponent = CTransform::Create(pOwner);
		break;

	case Client::COM_COLLIDER:
	{
		json ColliderJson = _json[JSON_COLLIDER_INFO];
		_float4 vOffsetPos = CUtility_Json::Get_VectorFromJson(ColliderJson, "vOffsetPos");

		pComponent = CCollider_Sphere::Create(pOwner, ColliderJson["fRadius"], ColliderJson["iColIndex"], vOffsetPos);
	}

	break;

	case Client::COM_RENDERER:
		break;

	case Client::COM_PHYSICS:
		break;

	case Client::COM_TEXTURE:
		break;

	case Client::COM_SHADER:
		break;

	case Client::COM_END:
		break;

	default:
		break;
	}

	return pComponent;
}

