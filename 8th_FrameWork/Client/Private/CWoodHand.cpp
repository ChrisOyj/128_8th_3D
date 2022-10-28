#include "stdafx.h"
#include "CWoodHand.h"

#include "HIerarchyNode.h"
#include "GameInstance.h"

#include "Model.h"
#include "Physics.h"
#include "CUnit.h"
#include "CModel_Renderer.h"
#include "CShader.h"
#include "CAnimator.h"

#include "Transform.h"
#include "CComponent_Factory.h"

#include "CCamera_Follow.h"

#include "CUtility_Transform.h"

#include "Texture.h"

CWoodHand::CWoodHand()
{
}

CWoodHand::~CWoodHand()
{
}

CWoodHand* CWoodHand::Create()
{
	CWoodHand* pInstance = new CWoodHand;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CWoodHand");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}


HRESULT CWoodHand::Initialize_Prototype()
{
	m_eShaderType = SHADER_VTXANIMMODEL;
	m_eDisableType = NONE;
	m_fFadeOutStartTime = 1.5f;
	m_wstrPath = L"../bin/resources/meshes/effects/NinJutsu/woodhand/WoodHandShort_L.fbx";

	m_matTrans = XMMatrixScaling(0.007f, 0.007f, 0.007f) * XMMatrixRotationZ(XMConvertToRadians(-90.0f));
	m_hcMyCode = HASHCODE(CWoodHand);
	m_iPassType = VTXANIM_PASS_NORMAL;
	m_vEffectFlag = SH_LIGHT_NOSPEC;

	m_vOffsetPos = _float4(-2.f, 0.f, 0.f);

	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXANIMMODEL,
		VTXANIM_DECLARATION::Element, VTXANIM_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	CModel_Renderer* pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_NONALPHA, m_iPassType
		, _float4(0.f, 0.f, 0.f, 1.f));

	Add_Component<CRenderer>(pRenderer);
	CModel* pModelCom = CModel::Create(CP_BEFORE_RENDERER, TYPE_ANIM, L"../bin/resources/meshes/effects/NinJutsu/woodhand/WoodHandShort_L_Ninjutsu_LaughingMonk_End.fbx", m_matTrans);
	Add_Component(pModelCom);

	CAnimator* pAnimator = CAnimator::Create(CP_BEFORE_RENDERER, m_wstrPath);
	Add_Component(pAnimator);

	CTexture* pTextureCom = CTexture::Create(0, L"../bin/resources/meshes/textures/animmodeltextures/T_ITM_WoodHandShort_N.dds", 1);

	pModelCom->Get_Materials().front().second.pTextures[aiTextureType_NORMALS] = pTextureCom;

	return S_OK;
}

HRESULT CWoodHand::Start()
{
	CGameObject::Start();

	GET_COMPONENT(CAnimator)->Set_CurAnimIndex(0, 0);
	GET_COMPONENT(CAnimator)->Set_InterpolationTime(0, 0, 0);
	GET_COMPONENT(CAnimator)->Set_AnimSpeed(0, 0, 1.f);

	return S_OK;
}

void CWoodHand::My_LateTick()
{
	if (GET_COMPONENT(CAnimator)->Is_CurAnimFinished())
	{
		//DISABLE_GAMEOBJECT(this);
	}
}

void CWoodHand::OnEnable()
{
	GET_COMPONENT(CAnimator)->Set_CurAnimIndex(0, 0);
	GET_COMPONENT(CAnimator)->Set_InterpolationTime(0, 0, 0);
	GET_COMPONENT(CAnimator)->Set_AnimSpeed(0, 0, 1.f);

	__super::OnEnable();
}
