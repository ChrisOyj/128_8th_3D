#include "stdafx.h"
#include "CWoodWall.h"

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
#include "CEffects_Factory.h"
#include "CCollider_Sphere.h"

#include "Functor.h"

CWoodWall::CWoodWall()
{
}

CWoodWall::~CWoodWall()
{
}

CWoodWall* CWoodWall::Create()
{
	CWoodWall* pInstance = new CWoodWall;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CWoodWall");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CWoodWall::OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos)
{
	if (eColType == COL_PLAYERSKILL || eColType == COL_ENEMYSKILL || eColType == COL_PLAYERSPECIAL)
		DISABLE_GAMEOBJECT(this);
}


HRESULT CWoodWall::Initialize_Prototype()
{
	m_eShaderType = SHADER_VTXANIMMODEL;
	m_eDisableType = FADE;
	m_fFadeOutStartTime = 7.f;
	m_wstrPath = L"../bin/resources/meshes/effects/NinJutsu/woodhand/SM_WEP_Eff_DomedWall_A_01_Anim.fbx";

	m_matTrans = XMMatrixScaling(0.08f, 0.08f, 0.08f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	m_hcMyCode = HASHCODE(CWoodWall);
	m_iPassType = VTXANIM_PASS_NORMAL;
	m_vEffectFlag = SH_LIGHT_NOSPEC;

	//m_vOffsetPos = _float4(-2.f, 0.f, 0.f);

	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXANIMMODEL,
		VTXANIM_DECLARATION::Element, VTXANIM_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	CModel_Renderer* pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_NONALPHA, m_iPassType
		, _float4(0.f, 0.f, 0.f, 1.f));

	Add_Component<CRenderer>(pRenderer);
	CModel* pModelCom = CModel::Create(CP_BEFORE_RENDERER, TYPE_ANIM, L"../bin/resources/meshes/effects/NinJutsu/woodhand/SM_WEP_Eff_DomedWall_A_01.fbx", m_matTrans);
	Add_Component(pModelCom);

	CAnimator* pAnimator = CAnimator::Create(CP_BEFORE_RENDERER, m_wstrPath);
	Add_Component(pAnimator);

	CTexture* pTextureCom = CTexture::Create(0, L"../bin/resources/meshes/textures/animmodeltextures/T_WEP_Eff_DomedWall_01_N.dds", 1);
	pModelCom->Get_Materials().front().second.pTextures[aiTextureType_NORMALS] = pTextureCom;


	/*pTextureCom = pModelCom->Get_Materials().front().second.pTextures[aiTextureType_DIFFUSE];

	pTextureCom->Add_Texture(L"../bin/resources/meshes/textures/animmodeltextures/T_EFF_GMS_Sand_04_BC.png");
	pTextureCom->Set_CurTextureIndex(1);*/

	CCollider_Sphere* pCollider = CComponent_Factory::Clone_Component<CCollider_Sphere>(this);
	
	Add_Component(pCollider);

	return S_OK;
}

HRESULT CWoodWall::Initialize()
{
	CCollider_Sphere* pCollider = GET_COMPONENT(CCollider_Sphere);
	pCollider->Initialize();
	pCollider->Get_ColInfo().fRadius = 3.2f;
	pCollider->Get_ColInfo().vOffsetPos = _float4(0.f, 0.f, 2.f);
	pCollider->Set_ColIndex(COL_WALL);
	return S_OK;
}

HRESULT CWoodWall::Start()
{
	CGameObject::Start();

	GET_COMPONENT(CAnimator)->Set_CurAnimIndex(0, 0);
	GET_COMPONENT(CAnimator)->Set_InterpolationTime(0, 0, 0);
	GET_COMPONENT(CAnimator)->Set_AnimSpeed(0, 0, 1.f);

	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"WoodWallSmoke", this, m_pTransform->Get_MyWorld(WORLD_POS));

	CallBack_CollisionEnter += bind(&CWoodWall::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3);

	return S_OK;
}

void CWoodWall::My_LateTick()
{

	if (!m_bOnce && GET_COMPONENT(CAnimator)->Is_CurAnimFinished())
	{
		CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"WoodWallParticle_0"), this, m_pTransform->Get_MyWorld(WORLD_POS));
		m_bOnce = true;
		DISABLE_COMPONENT(GET_COMPONENT(CAnimator));
	}

	__super::My_LateTick();
}

void CWoodWall::OnEnable()
{
	GET_COMPONENT(CAnimator)->Set_CurAnimIndex(0, 0);
	GET_COMPONENT(CAnimator)->Set_InterpolationTime(0, 0, 0);
	GET_COMPONENT(CAnimator)->Set_AnimSpeed(0, 0, 1.f);
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"WoodWallSmoke", this, m_pTransform->Get_MyWorld(WORLD_POS));
	m_bOnce = false;
	__super::OnEnable();
}

void CWoodWall::OnDisable()
{
	CFunctor::Play_Sound(L"Eff_DomeBreak", CHANNEL_EFFECTS, m_pTransform->Get_World(WORLD_POS));

	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"WoodWallSmoke", this, m_pTransform->Get_MyWorld(WORLD_POS));
	_float4 vPos = m_pTransform->Get_MyWorld(WORLD_POS);
	_float4 vLook = m_pTransform->Get_MyWorld(WORLD_LOOK);
	vPos += vLook * 1.5f;
	vPos.y += 1.5f;
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"WoodWallParticle"), vPos);

	__super::OnDisable();
}
