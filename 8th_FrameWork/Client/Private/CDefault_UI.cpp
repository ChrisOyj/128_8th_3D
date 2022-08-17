#include "stdafx.h"
#include "CDefault_UI.h"

#include "GameInstance.h"

#include "Texture.h"
#include "CShader.h"
#include "Renderer.h"
#include "Transform.h"
#include "Functor.h"
#include "CFader.h"
#include "CMesh_Rect.h"
#include "Physics.h"

#include "CComponent_Factory.h"
#include "CPrototype_Factory.h"

#include "CUtility_Json.h"

CDefault_UI::CDefault_UI()
{
}

CDefault_UI::~CDefault_UI()
{
}

CDefault_UI* CDefault_UI::Clone_WithJson(wstring strFileKey)
{
	CDefault_UI* pInstance = CGameInstance::Get_Instance()->Clone_GameObject<CDefault_UI>();

	if (!pInstance)
		return nullptr;

	pInstance->Set_FileKey(strFileKey);

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CDefault_UI");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CDefault_UI::Initialize_Prototype()
{
	Add_Component(CComponent_Factory::Clone_Component<CShader>(this));
	Add_Component<CMesh>(CComponent_Factory::Clone_Component<CMesh_Rect>(this));
	Add_Component(CComponent_Factory::Clone_Component<CFader>(this));
	Add_Component(CComponent_Factory::Clone_Component<CPhysics>(this));
	Add_Component(CComponent_Factory::Clone_Component<CTexture>(this));
	Add_Component(CComponent_Factory::Clone_Component<CRenderer>(this));

	m_strFileKey = L"0";
	

	return S_OK;
}

HRESULT CDefault_UI::Initialize()
{
	m_pTransform->Get_Transform().vScale = _float4(100.f, 100.f, 0.f);

	json	UIJson;

	if (FAILED(CUtility_Json::Load_Json(CUtility_Json::Complete_Path(m_strFileKey).c_str(), &UIJson)))
		return S_OK;

	TRANSFORM& tTransform = m_pTransform->Get_Transform();
	tTransform.matMyWorld = CUtility_Json::Get_MatrixFromJson(UIJson["matWorld"]);
	tTransform.vScale = CUtility_Json::Get_VectorFromJson(UIJson["vScale"]);

	GET_COMPONENT(CFader)->Get_FadeDesc() = CUtility_Json::Get_StructFromJson<FADEDESC>(UIJson["tFadeDesc"]);
	//_uint iCurPass = UIJson["iCurPass"];
	GET_COMPONENT(CRenderer)->Set_Pass(UIJson["iCurPass"]);
	GET_COMPONENT(CPhysics)->Get_Physics() = CUtility_Json::Get_StructFromJson<PHYSICS>(UIJson["tPhysics"]);

	//_uint iCurTextureIndex = UIJson["iCurTextureIndex"];
	GET_COMPONENT(CTexture)->Get_vecTexture().clear();

	for (_uint i = 0; i < UIJson["strTextureFilePath"].size(); ++i)
	{
		wstring strFilePath = UIJson["strTextureFilePath"][i];
		if (FAILED(GET_COMPONENT(CTexture)->Add_Texture(strFilePath.c_str())))
			return E_FAIL;
	}

	_uint iCurTextureIndex = UIJson["iCurTextureIndex"];

	GET_COMPONENT(CTexture)->Set_CurTextureIndex(0);


	return S_OK;
}

void CDefault_UI::OnEnable()
{
}

void CDefault_UI::OnDisable()
{
}
