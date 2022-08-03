#include "stdafx.h"
#include "CComponent_Factory.h"

#include "GameInstance.h"

#include "Transform.h"
#include "CCollider_Sphere.h"

#include "CUtility_Json.h"
#include "GameObject.h"
#include "Texture.h"
#include "CMesh.h"
#include "Renderer.h"


#define JSON_COMPONENT_TYPE	"Component_Type"
#define JSON_COLLIDER_INFO	"Collider_Info"
#define JSON_RENDERER_INFO	"Renderer_Info"
#define JSON_TEXTURE_INFO	"Texture_Info"
#define JSON_MESH_INFO		"Mesh_Info"

CComponent* CComponent_Factory::Create_FromJson(const _uint& iID, CGameObject* pOwner)
{
	CComponent* pComponent = CLONE_COMPONENT(iID);

	if (!pComponent)
	{
		pComponent = Create_PrototypeFromJson(iID)->Clone();

		if (!pComponent)
			return nullptr;
	}

	pComponent->Set_Owner(pOwner);

	if (FAILED(pComponent->Initialize()))
	{
		Call_MsgBox_Index(L"Failed to Initialize : Index ->", iID);
		return nullptr;
	}

	return pComponent;
}

CComponent* CComponent_Factory::Create_FromPrototype(const _uint& iID, CGameObject* pOwner)
{
	CComponent* pComponent = CLONE_COMPONENT(iID);

	if (!pComponent)
	{
		return nullptr;
	}

	pComponent->Set_Owner(pOwner);

	if (FAILED(pComponent->Initialize()))
	{
		Call_MsgBox_Index(L"Failed to Initialize : Index ->", iID);
		return nullptr;
	}

	return pComponent;
}

CComponent* CComponent_Factory::Create_PrototypeFromJson(const _uint& iComponentID)
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

	pComponent = Create_InstanceFromJson(ComponentJson);

	if (!pComponent)
	{
		Call_MsgBox_Index(L"Failed to Create_InstanceFromJson : CComponent_Factory", iComponentID);
		return nullptr;
	}

	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(iComponentID, pComponent)))
		return nullptr;


	return pComponent->Clone();
}

CComponent* CComponent_Factory::Create_InstanceFromJson(const json& _json)
{
	COMPONENT_TYPE	eComponentID = _json[JSON_COMPONENT_TYPE];

	CComponent* pComponent = nullptr;
	_uint iGroupID = _json["iGroupID"];

	switch (eComponentID)
	{
	case Client::COM_TRANSFORM:
		pComponent = CTransform::Create(iGroupID);
		break;

	case Client::COM_COLLIDER:
	{
		json ColliderJson = _json[JSON_COLLIDER_INFO];
		_float4 vOffsetPos = CUtility_Json::Get_VectorFromJson(ColliderJson, "vOffsetPos");

		pComponent = CCollider_Sphere::Create(iGroupID, ColliderJson["fRadius"], ColliderJson["iColIndex"], vOffsetPos);
	}

	break;

	case Client::COM_RENDERER:
	{
		json RendererJson = _json[JSON_RENDERER_INFO];
		_float4 vOffsetPos = CUtility_Json::Get_VectorFromJson(RendererJson, "vOffsetPos");

		pComponent = CRenderer::Create(iGroupID, RendererJson["eRenderGroup"], RendererJson["iCurPass"], vOffsetPos);
	}
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

