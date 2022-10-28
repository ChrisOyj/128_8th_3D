#include "stdafx.h"
#include "CChakraJump_Guide.h"

#include "HIerarchyNode.h"
#include "GameInstance.h"

#include "Model.h"
#include "Physics.h"
#include "CUnit.h"
#include "CModel_Renderer.h"
#include "CShader.h"
#include "Texture.h"
#include "CEffects_Factory.h"

#include "Transform.h"
#include "CComponent_Factory.h"

#include "CCamera_Follow.h"

#include "CUtility_Transform.h"

CChakraJump_Guide::CChakraJump_Guide()
{
}

CChakraJump_Guide::~CChakraJump_Guide()
{
}

CChakraJump_Guide* CChakraJump_Guide::Create()
{
	CChakraJump_Guide* pInstance = new CChakraJump_Guide;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CChakraJump_Guide");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CChakraJump_Guide::Reset(_float4 vStartPos)
{
	__super::Reset(vStartPos);
	
		m_pLoopEffect = CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"CJ_LoopEffect_0"), vStartPos);
		static_cast<CEffect*>(m_pLoopEffect)->Set_EffectFlag(0);
	

	LIGHTDESC			LightDesc;

	LightDesc.eType = tagLightDesc::TYPE_POINT;

	LightDesc.fRange = 4.f;

	LightDesc.vDiffuse = _float4(0.1f, 0.2f, 0.6f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.pOwner = m_pLoopEffect;
	LightDesc.vOffset.y = 0.5f;

	GAMEINSTANCE->Add_Light(LightDesc);

}

HRESULT CChakraJump_Guide::Initialize_Prototype()
{
	m_eDisableType = NONE;
	m_wstrPath = L"../bin/resources/meshes/effects/common/SM_PRP_Eff_NinjaJumpGuid.fbx";
	m_matTrans = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	m_fUVSpeedY = 1.f;
	m_iPassType = 0;
	m_hcMyCode = HASHCODE(CChakraJump_Guide);
	__super::Initialize_Prototype();

	GET_COMPONENT(CModel)->Get_Materials().front().second.pTextures[aiTextureType_DIFFUSE_ROUGHNESS]->Add_Texture(L"../bin/resources/textures/Effects/GradationColor/T_EFF_GMS_GuardianArmor_01_01_BC.png");
	GET_COMPONENT(CModel)->Get_Materials().front().second.pTextures[aiTextureType_DIFFUSE_ROUGHNESS]->Set_CurTextureIndex(1);

	return S_OK;

}

HRESULT CChakraJump_Guide::Initialize()
{

	return __super::Initialize();
}

void CChakraJump_Guide::My_Tick()
{
	if (KEY(C, NONE))
	{
		DISABLE_GAMEOBJECT(this);
		return;
	}

	_float4 vLook = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
	_float4 vRight = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_RIGHT);

	vLook.y = 0.f;
	vLook.Normalize();
	vRight.y = 0.f;
	vRight.Normalize();

	_float4 vFinalDir = { 0.f, 0.f, 0.f, 0.f };

	if (KEY(W, HOLD))
	{
		vFinalDir += vLook;
	}
	if (KEY(A, HOLD))
	{
		vFinalDir -= vRight;
	}
	if (KEY(S, HOLD))
	{
		vFinalDir -= vLook;
	}
	if (KEY(D, HOLD))
	{
		vFinalDir += vRight;
	}


	if (vFinalDir.Is_Zero())
	{
		m_pTransform->Set_Look(vLook);
		CUtility_Transform::Turn_ByAngle(m_pTransform, m_pTransform->Get_MyWorld(WORLD_RIGHT), -90.f);
	}
	else
	{
		m_pTransform->Set_Look(vFinalDir);
		CUtility_Transform::Turn_ByAngle(m_pTransform, m_pTransform->Get_MyWorld(WORLD_RIGHT), -60.f);
	}

}

void CChakraJump_Guide::OnEnable()
{
	__super::OnEnable();

	m_fAlpha = 1.f;
}

void CChakraJump_Guide::OnDisable()
{
	__super::OnDisable();

	if (m_pLoopEffect)
	{
		DISABLE_GAMEOBJECT(m_pLoopEffect);
		m_pLoopEffect = nullptr;
	}
}
