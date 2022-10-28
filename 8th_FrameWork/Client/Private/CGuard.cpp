#include "stdafx.h"
#include "CGuard.h"

#include "HIerarchyNode.h"
#include "GameInstance.h"

#include "Model.h"
#include "Physics.h"
#include "CUnit.h"
#include "CModel_Renderer.h"
#include "CShader.h"


#include "Transform.h"
#include "CComponent_Factory.h"

#include "CCamera_Follow.h"
#include "Texture.h"
#include "CUtility_Transform.h"

CGuard::CGuard()
{
}

CGuard::~CGuard()
{
}

CGuard* CGuard::Create()
{
	CGuard* pInstance = new CGuard;


	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CGuard");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CGuard::Start()
{
	__super::Start();

	return S_OK;
}

HRESULT CGuard::Initialize_Prototype()
{
	m_eDisableType = UV;
	m_wstrPath = L"../bin/resources/meshes/effects/common/SM_EFF_HemiSphere_A_01.fbx";
	m_matTrans = XMMatrixScaling(0.02f, 0.02f, 0.02f) * XMMatrixRotationZ(XMConvertToRadians(-45.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	m_fUVSpeedY = 3.f;
	m_iPassType = VTXEFFECT_PASS_DEFAULT;
	m_hcMyCode = HASHCODE(CGuard);
	m_bEffectFlag |= EFFECT_FOLLOWTARGET;
	m_vOffsetPos = _float4(0.f, 0.7f, 0.3f);
	m_vGlowFlag = _float4(1.f, 1.f, 1.f, 0.3f);

	m_vEffectFlag = SH_EFFECT_NONE;

	__super::Initialize_Prototype();
	//colormap
	GET_COMPONENT(CModel)->Get_Materials().front().second.pTextures[aiTextureType_DIFFUSE_ROUGHNESS]->Add_Texture(L"../bin/resources/textures/Effects/GradationColor/T_EFF_GMS_Light_01_BC.png");
	GET_COMPONENT(CModel)->Get_Materials().front().second.pTextures[aiTextureType_DIFFUSE_ROUGHNESS]->Set_CurTextureIndex(1);

	return S_OK;
}

HRESULT CGuard::Initialize()
{
	return S_OK;
}