#include "stdafx.h"
#include "CEightGatesDragon.h"

#include "HIerarchyNode.h"
#include "GameInstance.h"

#include "CScript_FollowCam.h"
#include "CUser.h"
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

#include "ImGui_Manager.h"

#include "CEffects_Factory.h"

CEightGatesDragon::CEightGatesDragon()
{
}

CEightGatesDragon::~CEightGatesDragon()
{
}

CEightGatesDragon* CEightGatesDragon::Create()
{
	CEightGatesDragon* pInstance = new CEightGatesDragon;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CEightGatesDragon");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CEightGatesDragon::Start_Dragon()
{
	GAMEINSTANCE->Change_Camera(L"PlayerCam");
	m_pFollowTarget = CUser::Get_Instance()->Get_FollowCam()->Get_Owner();
	GET_COMPONENT(CAnimator)->Set_AnimSpeed(0, 0, 0.01f);


	m_vOffsetPos = _float4(-1.5f, -153.f, -38.f);

}


HRESULT CEightGatesDragon::Initialize_Prototype()
{
	m_eShaderType = SHADER_VTXANIMMODEL;
	m_eDisableType = FADE;

	m_fFadeInStartTime = 0.f;
	m_fFadeInTime = 0.5f;
	m_fFadeOutStartTime = 2.3f;
	m_fFadeOutTime = 5.f;
	m_fUVSpeedX = 0.5f;
	m_fUVSpeedY = 0.5f;
	m_vEffectFlag = SH_EFFECT_NOBLOOM;
	m_vGlowFlag = _float4(1.f, 0.3f, 0.3f, 1.f);

	m_vOffsetPos = _float4(-1.5f, -153.f, -43.5f);


	m_wstrPath = L"../bin/resources/meshes/effects/NinJutsu/Dragon/D28_Ninjutsu_D28NJ3_Dragon_AttackLoop.fbx";

	m_matTrans = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationX(XMConvertToRadians(-150.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	m_hcMyCode = HASHCODE(CEightGatesDragon);
	m_iPassType = VTXANIM_PASS_DRAGON;

	m_bEffectFlag = EFFECT_FOLLOWTARGET;


	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXANIMMODEL,
		VTXANIM_DECLARATION::Element, VTXANIM_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	CModel_Renderer* pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_ALPHA, m_iPassType
		, _float4(0.f, 0.f, 0.f, 1.f));

	Add_Component<CRenderer>(pRenderer);
	CModel* pModelCom = CModel::Create(CP_BEFORE_RENDERER, TYPE_ANIM, L"../bin/resources/meshes/effects/NinJutsu/Dragon/D28_Ninjutsu_D28NJ3_Dragon_Attack.fbx", m_matTrans);
	Add_Component(pModelCom);

	CAnimator* pAnimator = CAnimator::Create(CP_BEFORE_RENDERER, m_wstrPath);
	Add_Component(pAnimator);


	CTexture* pTextureCom = CTexture::Create(0, L"../bin/resources/textures/effects/gradationcolor/T_EFF_GMS_Mad_01_BC.png", 1);

	pModelCom->Get_Materials().front().second.pTextures[aiTextureType_NORMALS] = pTextureCom;

	pTextureCom = CTexture::Create(0, L"../bin/resources/textures/effects/gradationcolor/T_EFF_GMS_Mad_01_BC.png", 1);
	pModelCom->Get_Materials().back().second.pTextures[aiTextureType_NORMALS] = pTextureCom;


	/*pTextureCom = CTexture::Create(0, L"../bin/resources/textures/modeltextures/T_ENV_KNVLLG_Noize_05_UC.dds", 1);
	pModelCom->Get_Materials().front().second.pTextures[aiTextureType_DIFFUSE_ROUGHNESS] = pTextureCom;

	pTextureCom = CTexture::Create(0, L"../bin/resources/textures/modeltextures/T_ENV_KNVLLG_Noize_05_UC.dds", 1);
	pModelCom->Get_Materials().back().second.pTextures[aiTextureType_DIFFUSE_ROUGHNESS] = pTextureCom;*/


	pModelCom->Get_Materials().front().second.pTextures[1]->Add_Texture(L"../bin/resources/textures/effects/NinJutsu/T_EFF_D28Dragon_A_M.dds");
	pModelCom->Get_Materials().front().second.pTextures[1]->Set_CurTextureIndex(1);

	pModelCom->Get_Materials().back().second.pTextures[1]->Add_Texture(L"../bin/resources/textures/effects/NinJutsu/T_EFF_D28Dragon_A_M.dds");
	pModelCom->Get_Materials().back().second.pTextures[1]->Set_CurTextureIndex(1);

	return S_OK;
}

HRESULT CEightGatesDragon::Start()
{
	__super::Start();


	GET_COMPONENT(CAnimator)->Set_CurAnimIndex(0, 0);
	GET_COMPONENT(CAnimator)->Set_InterpolationTime(0, 0, 0);
	GET_COMPONENT(CAnimator)->Set_AnimSpeed(0, 0, 0.001f);

#ifdef _DEBUG
	CImGui_Manager::Get_Instance()->SetUp_Dragon(this);
#endif // DEBUG

	
	m_pEffect = CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"NightGuy_Dragon_0"), m_pFollowTarget, m_pFollowTarget->Get_Transform()->Get_World(WORLD_POS));
	static_cast<CModel_Renderer*>(GET_COMPONENT_FROM(m_pEffect, CRenderer))->Set_ShadowType(CModel_Renderer::NOSHADOW);


	LIGHTDESC			LightDesc;

	LightDesc.eType = tagLightDesc::TYPE_POINT;

	LightDesc.fRange = 6.f;

	LightDesc.vDiffuse = _float4(0.5f, 0.f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(0.3f, 0.3f, 0.3f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.pOwner = this;
	LightDesc.vOffset = { 0.f, 0.6f, 0.f, 1.f };

	GAMEINSTANCE->Add_Light(LightDesc);

	return S_OK;
}

void CEightGatesDragon::My_LateTick()
{
	__super::My_LateTick();
	/*if (m_fFadeTimeAcc > 0.1f)
	{
		if (!GET_COMPONENT(CAnimator)->Is_Disable())
			DISABLE_COMPONENT(GET_COMPONENT(CAnimator));

	}*/

}

void CEightGatesDragon::OnEnable()
{
	GET_COMPONENT(CAnimator)->Set_CurAnimIndex(0, 0);
	GET_COMPONENT(CAnimator)->Set_InterpolationTime(0, 0, 0);
	GET_COMPONENT(CAnimator)->Set_AnimSpeed(0, 0, 0.001f);
	m_pEffect = CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"NightGuy_Dragon_0"), m_pFollowTarget, m_pFollowTarget->Get_Transform()->Get_World(WORLD_POS));
	LIGHTDESC			LightDesc;

	LightDesc.eType = tagLightDesc::TYPE_POINT;

	LightDesc.fRange = 6.f;

	LightDesc.vDiffuse = _float4(0.5f, 0.f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(0.3f, 0.3f, 0.3f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.pOwner = this;
	LightDesc.vOffset = { 0.f, 0.6f, 0.f, 1.f };

	GAMEINSTANCE->Add_Light(LightDesc);

	__super::OnEnable();
}

void CEightGatesDragon::OnDisable()
{
	DISABLE_GAMEOBJECT(m_pEffect);
}
