#include "stdafx.h"
#include "CComponent_Factory.h"

#include "GameInstance.h"

#include "Transform.h"
#include "CCollider_Sphere.h"

#include "CUtility_Json.h"
#include "GameObject.h"
#include "Texture.h"
#include "CMesh_Rect.h"
#include "Renderer.h"
#include "CFader.h"
#include "Physics.h"
#include "CShader.h"

#include "CPrototype_Factory.h"


#define JSON_COMPONENT_TYPE	"Component_Type"

HRESULT CComponent_Factory::Save_Json(const _uint& iID, CComponent* pComponent)
{
	_uint iSaveID = iID;
	json	SaveJson;
	if (Safe_CheckID(pComponent->m_iSaveID, ID_COMPONENT))
		iSaveID = pComponent->m_iSaveID;

	COMPONENT_TYPE	eComponentType = (COMPONENT_TYPE)pComponent->Get_ComponentID();
	SaveJson.emplace(JSON_COMPONENT_TYPE, (_uint)eComponentType);
	SaveJson.emplace("iGroupID", pComponent->Get_GroupID());

	switch (eComponentType)
	{
	case Client::COM_TRANSFORM:
		return E_FAIL;
		break;
	case Client::COM_COLLIDER:
		Save_Collider(pComponent, &SaveJson);
		break;
	case Client::COM_RENDERER:
		Save_Renderer(pComponent, &SaveJson);
		break;
	case Client::COM_PHYSICS:
		Save_Physics(pComponent, &SaveJson);
		break;
	case Client::COM_TEXTURE:
		Save_Texture(pComponent, &SaveJson);
		break;
	case Client::COM_SHADER:
		Save_Shader(pComponent, &SaveJson);
		break;
	case Client::COM_FADER:
		Save_Fader(pComponent, &SaveJson);
		break;
	case Client::COM_MESH:
		Save_Mesh(pComponent, &SaveJson);
		break;
	default:
		break;
	}

	CUtility_Json::Save_Json(CUtility_Json::Complete_Path(iSaveID).c_str(), SaveJson);

	return (iSaveID == iID) ? S_OK : E_FAIL;
}

CComponent* CComponent_Factory::Create_FromJson(const _uint& iID, CGameObject* pOwner)
{
	CComponent* pComponent = CLONE_COMPONENT(iID);

	if (!pComponent)
	{
		pComponent = Create_PrototypeFromJson(iID);

		if (!pComponent)
			return nullptr;
	}

	pComponent->Set_Owner(pOwner);

	if (FAILED(pComponent->Initialize()))
	{
		Call_MsgBox_Index(L"Failed to Initialize : Index ->", iID);
		return nullptr;
	}

	pComponent->m_iSaveID = iID;

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

	pComponent->m_iSaveID = iID;

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

	json ComponentJson;

	if (FAILED(CUtility_Json::Load_Json(CUtility_Json::Complete_Path(iComponentID).c_str(), &ComponentJson)))
	{
		Call_MsgBox_Index(L"Failed to Load_Json : CLevel_Default", iComponentID);
		return nullptr;
	}

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
		break;

	case Client::COM_COLLIDER:
	{
		_float4 vOffsetPos = CUtility_Json::Get_VectorFromJson(_json["vOffsetPos"]);

		pComponent = CCollider_Sphere::Create(iGroupID, _json["fRadius"], _json["iColIndex"], vOffsetPos);
	}

	break;

	case Client::COM_RENDERER:
	{
		_float4 vOffsetPos = CUtility_Json::Get_VectorFromJson(_json["vOffsetPos"]);

		pComponent = CRenderer::Create(iGroupID, _json["eRenderGroup"], _json["iCurPass"], vOffsetPos);
	}
		break;

	case Client::COM_PHYSICS:
	{
		pComponent = CGameInstance::Get_Instance()->Find_Component_Prototype(CPrototype_Factory::DEFAULT_PHYSICS);
		CPhysics* pPhyscis = static_cast<CPhysics*>(pComponent);
		pPhyscis->Get_Physics().fTurnSpeed = _json["fTurnSpeed"];
		pPhyscis->Get_Physics().vTurnDir = CUtility_Json::Get_VectorFromJson(_json["vTurnDir"]);
	}

		break;

	case Client::COM_TEXTURE:
	{
		json TexturePathJson = _json["strFilePath"];
		wstring strFirstFilePath = TexturePathJson[0];
		CTexture* pTexture = CTexture::Create(iGroupID, strFirstFilePath.c_str(), 1);
		_uint iTexIdx = _json["iCurTextureIndex"];
		if (iTexIdx >= pTexture->Get_TextureSize())
		{
			iTexIdx = pTexture->Get_TextureSize() - 1;
		}
		pTexture->Set_CurTextureIndex(iTexIdx);
		for (_uint i = 1; i < TexturePathJson.size(); ++i)
		{
			wstring strFilePath = TexturePathJson[i];
			pTexture->Add_Texture(strFilePath.c_str());
		}

		pComponent = pTexture;
	}
		break;

	case Client::COM_SHADER:
	{
		SHADER_FILE_ID eShaderFileType = (SHADER_FILE_ID)_json["iShaderFileIndex"];

		switch (eShaderFileType)
		{
		case SHADER_VTXTEX:
			pComponent = CGameInstance::Get_Instance()->Find_Component_Prototype(CPrototype_Factory::DEFAULT_SHADER);

			break;
		default:
			break;
		}
	}
		break;

	case Client::COM_FADER:
	{
		FADEDESC	tFadeDesc = CUtility_Json::Get_StructFromJson<FADEDESC>(_json["tFadeDesc"]);
		CFader* pFader = CFader::Create((COMPONENT_PIPELINE)iGroupID, tFadeDesc);
		pComponent = pFader;
	}
		break;

	case COM_MESH:
	{
		MESH_TYPE eMeshType = _json["iMeshType"];
		switch (eMeshType)
		{
		case MESH_RECT:
			pComponent = CGameInstance::Get_Instance()->Find_Component_Prototype(CPrototype_Factory::DEFAULT_MESH_RECT);
			break;
		default:
			pComponent = nullptr;
			break;
		}
	}
	break;

	default:
		break;
	}
	

	return pComponent;
}

void CComponent_Factory::Save_Transform(CComponent* pComponent, json* pOut)
{
	CTransform* pTransform = dynamic_cast<CTransform*>(pComponent);

	if (!pTransform)
	{
		Call_MsgBox(L"Failed to Save_Transform : CComponent_Factory");
		return;
	}

	pOut->emplace("WorldMatrix", pTransform->Get_Transform().matMyWorld.m);
}

void CComponent_Factory::Save_Collider(CComponent* pComponent, json* pOut)
{
}

void CComponent_Factory::Save_Renderer(CComponent* pComponent, json* pOut)
{
	CRenderer* pRenderer = dynamic_cast<CRenderer*>(pComponent);

	if (!pRenderer)
	{
		Call_MsgBox(L"Failed to Save_Renderer : CComponent_Factory");
		return;
	}

	pOut->emplace("eRenderGroup", (_uint)pRenderer->Get_RenderGroup());
	pOut->emplace("vOffsetPos", pRenderer->Get_OffsetPos().XMLoad().m128_f32);
	pOut->emplace("iCurPass", pRenderer->Get_Pass());
	
}

void CComponent_Factory::Save_Physics(CComponent* pComponent, json* pOut)
{
	CPhysics* pPhysics = dynamic_cast<CPhysics*>(pComponent);

	if (!pPhysics)
	{
		Call_MsgBox(L"Failed to Save_Physics : CComponent_Factory");
		return;
	}

	pOut->emplace("vTurnDir", pPhysics->Get_Physics().vTurnDir.XMLoad().m128_f32);
	pOut->emplace("fTurnSpeed", pPhysics->Get_Physics().fTurnSpeed);
}

void CComponent_Factory::Save_Texture(CComponent* pComponent, json* pOut)
{
	CTexture* pTexture = dynamic_cast<CTexture*>(pComponent);

	if (!pTexture)
	{
		Call_MsgBox(L"Failed to Save_Texture : CComponent_Factory");
		return;
	}

	pOut->emplace("iCurTextureIndex", pTexture->Get_CurTextureIndex());

	json TextureFilePathJson;
	vector<TEXTUREDESC>& vecTextures = pTexture->Get_vecTexture();
	for (_uint i = 0; i < vecTextures.size(); ++i)
	{
		if (i == 0)
		{
			wstring wstrFullPath = vecTextures[i].strFilePath;
			string strFullPath;
			strFullPath.assign(wstrFullPath.begin(), wstrFullPath.end());

			_int iFind = (_int)strFullPath.rfind("\\") + 1;
			string strFileName = strFullPath.substr(iFind, strFullPath.length() - iFind);

			if (strFileName == "Jusin_0.png");
			continue;
		}
		

		TextureFilePathJson.push_back(vecTextures[i].strFilePath);
	}
	pOut->emplace("strFilePath", TextureFilePathJson);
}

void CComponent_Factory::Save_Shader(CComponent* pComponent, json* pOut)
{
	CShader* pShdaer = dynamic_cast<CShader*>(pComponent);

	if (!pShdaer)
	{
		Call_MsgBox(L"Failed to Save_Shader : CComponent_Factory");
		return;
	}

	pOut->emplace("iShaderFileIndex", pShdaer->Get_ShaderFileIndex());
}

void CComponent_Factory::Save_Fader(CComponent* pComponent, json* pOut)
{
	CFader* pFader = dynamic_cast<CFader*>(pComponent);

	if (!pFader)
	{
		Call_MsgBox(L"Failed to Save_Fader : CComponent_Factory");
		return;
	}

	FADEDESC& tFadeDesc = pFader->Get_FadeDesc();
	CUtility_Json::Save_Struct("tFadeDesc", tFadeDesc, pOut);

}

void CComponent_Factory::Save_Mesh(CComponent* pComponent, json* pOut)
{
	CMesh* pMesh = dynamic_cast<CMesh*>(pComponent);

	if (!pMesh)
	{
		Call_MsgBox(L"Failed to Save_Mesh : CComponent_Factory");
		return;
	}

	pOut->emplace("iMeshType", pMesh->Get_MeshType());
}

