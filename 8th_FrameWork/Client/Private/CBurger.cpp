#include "stdafx.h"
#include "CBurger.h"

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
#include "CCamera_Cinema.h"

#include "CEffects_Factory.h"
#include "CUser.h"

#include "Functor.h"

CBurger::CBurger()
{
}

CBurger::~CBurger()
{
}

CBurger* CBurger::Create()
{
	CBurger* pInstance = new CBurger;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CBurger");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}


HRESULT CBurger::Initialize_Prototype()
{
	m_eShaderType = SHADER_VTXANIMMODEL;
	m_eDisableType = FADE;

	m_fFadeInStartTime = 1.1f;
	m_fFadeInTime = 0.2f;
	m_vFadeInTargetScale = _float4(1.f, 1.f, 1.f);
	m_vStartScale = _float4(0.01f, 0.01f, 0.01f);
	m_fFadeOutStartTime = 9999.f;

	m_wstrPath = L"../bin/resources/meshes/Characters/burger/ML_075_020_Gamabunta_Anim.fbx";

	m_matTrans = XMMatrixScaling(0.003f, 0.003f, 0.003f) * XMMatrixRotationX(XMConvertToRadians(90.0f)) * XMMatrixRotationZ(XMConvertToRadians(180.0f));
	m_hcMyCode = HASHCODE(CBurger);
	m_iPassType = VTXANIM_PASS_NORMAL;
	m_vEffectFlag = SH_LIGHT_NOSPEC;
	m_bEffectFlag = 0;

	m_vOffsetPos = _float4(0.f, 0.f, 0.f);

	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXANIMMODEL,
		VTXANIM_DECLARATION::Element, VTXANIM_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	CModel_Renderer* pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_NONALPHA, m_iPassType
		, _float4(0.f, 0.f, 0.f, 1.f));

	Add_Component<CRenderer>(pRenderer);
	CModel* pModelCom = CModel::Create(CP_BEFORE_RENDERER, TYPE_ANIM, L"../bin/resources/meshes/Characters/burger/ML_075_020_Gamabunta.fbx", m_matTrans);
	Add_Component(pModelCom);

	CAnimator* pAnimator = CAnimator::Create(CP_BEFORE_RENDERER, m_wstrPath);
	Add_Component(pAnimator);


	CTexture* pTextureCom = CTexture::Create(0, L"../bin/resources/meshes/textures/animmodeltextures/T_CHR_Gamabunta_N.dds", 1);
	pModelCom->Get_Materials().front().second.pTextures[aiTextureType_NORMALS] = pTextureCom;

	return S_OK;
}

HRESULT CBurger::Start()
{
	CGameObject::Start();

	GET_COMPONENT(CAnimator)->Set_CurAnimIndex(0, 0);
	GET_COMPONENT(CAnimator)->Set_InterpolationTime(0, 0, 0);
	GET_COMPONENT(CAnimator)->Set_AnimSpeed(0, 0, 1.f);

	return S_OK;
}

void CBurger::My_Tick()
{
	/*if (KEY(NUM1, TAP))
	{
		m_fFadeTimeAcc = 0.f;
		m_eCurFadeType = FADEINREADY;
		m_bEnter = false;

		CUser::Get_Instance()->Get_SkillCam()->Set_Parent(this);
		CUser::Get_Instance()->Get_SkillCam()->Set_FollowTarget(this);
	}*/
}

void CBurger::My_LateTick()
{
	__super::My_LateTick();

	if (!m_bEnter && m_eCurFadeType == FADEIN)
	{
		GAMEINSTANCE->Play_Sound(L"Eff_BigSummon", (CHANNEL_GROUP)CHANNEL_EFFECTS);
		m_bEnter = true;
		GET_COMPONENT(CAnimator)->Set_CurAnimIndex(0, 0);
		GET_COMPONENT(CAnimator)->Set_InterpolationTime(0, 0, 0);
		GET_COMPONENT(CAnimator)->Set_AnimSpeed(0, 0, 1.f);
		static_cast<CCamera_Cinema*>(GAMEINSTANCE->Get_CurCam())->Start_ShakingCamera(0.15f, 0.4f);
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"BurgerStart", this, m_pTransform->Get_World(WORLD_POS));
		CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"BurgerLeaves"), m_pTransform->Get_World(WORLD_POS));

	}
	

	/*if (GET_COMPONENT(CAnimator)->Is_CurAnimFinished())
	{
		GET_COMPONENT(CAnimator)->Set_CurFrame(240);
	}*/
}

void CBurger::OnEnable()
{
	GET_COMPONENT(CAnimator)->Set_CurAnimIndex(0, 0);
	GET_COMPONENT(CAnimator)->Set_InterpolationTime(0, 0, 0);
	GET_COMPONENT(CAnimator)->Set_AnimSpeed(0, 0, 1.f);

	__super::OnEnable();
}
