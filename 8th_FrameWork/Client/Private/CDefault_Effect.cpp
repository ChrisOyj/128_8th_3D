#include "stdafx.h"
#include "CDefault_Effect.h"


#include "HIerarchyNode.h"
#include "GameInstance.h"

#include "Model.h"
#include "Physics.h"
#include "CUnit.h"
#include "CModel_Renderer.h"
#include "CShader.h"
#include "CUser.h"
#include "CUnit_Player.h"

#include "Transform.h"
#include "CComponent_Factory.h"

#include "CCamera_Follow.h"
#include "CEffects_Factory.h"
#include "CUtility_Transform.h"
#include "CUtility_File.h"
#include "Functor.h"

CDefault_Effect::CDefault_Effect()
{
}

CDefault_Effect::~CDefault_Effect()
{
}

CDefault_Effect* CDefault_Effect::Create()
{
	CDefault_Effect* pInstance = new CDefault_Effect;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CDefault_Effect");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

CDefault_Effect* CDefault_Effect::Create(ifstream* pReadFile)
{
	CDefault_Effect* pInstance = new CDefault_Effect;

	if (FAILED(pInstance->SetUp_DefaultEffect(pReadFile)))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CDefault_Effect");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CDefault_Effect::Initialize_Prototype()
{
	m_eDisableType = NONE;
	m_wstrPath = L"../bin/resources/meshes/effects/common/SM_EFF_HemiSphere_A_01.fbx";
	m_matTrans = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	m_hcMyCode = HASHCODE(CDefault_Effect);
	m_bEffectFlag |= EFFECT_FOLLOWTARGET;
	m_pFollowTarget = PLAYER;
	m_vOffsetPos = _float4(0.f, 0.f, 0.f);

	CPhysics* pPhysicsCom = CComponent_Factory::Clone_Component<CPhysics>(this);
	pPhysicsCom->Set_NaviOn();
	Add_Component(pPhysicsCom);

	return __super::Initialize_Prototype();
}

HRESULT CDefault_Effect::SetUp_DefaultEffect(ifstream* pReadFile)
{
	string	strModelPath;
	strModelPath = CUtility_File::Read_Text(pReadFile);

	m_wstrPath = CFunctor::To_Wstring(strModelPath);

	string	strMaskMapPath = CUtility_File::Read_Text(pReadFile);
	string	strColorMapPath = CUtility_File::Read_Text(pReadFile);

	m_wstrMaskMapPath = CFunctor::To_Wstring(strMaskMapPath);
	m_wstrColorMapPath = CFunctor::To_Wstring(strColorMapPath);

	pReadFile->read((char*)&m_eShaderType, sizeof(_uint));
	pReadFile->read((char*)&m_vEffectFlag, sizeof(_float4));
	pReadFile->read((char*)&m_vGlowFlag, sizeof(_float4));
	pReadFile->read((char*)&m_fUVSpeedX, sizeof(_float));
	pReadFile->read((char*)&m_fUVSpeedY, sizeof(_float));
	pReadFile->read((char*)&m_matTrans, sizeof(_float4x4));
	pReadFile->read((char*)&m_eDisableType, sizeof(_uint));
	pReadFile->read((char*)&m_iPassType, sizeof(_uint));
	pReadFile->read((char*)&m_bEffectFlag, sizeof(_byte));
	pReadFile->read((char*)&m_vTurnDir, sizeof(_float4));
	pReadFile->read((char*)&m_fTurnSpeed, sizeof(_float));
	pReadFile->read((char*)&m_vOffsetPos, sizeof(_float4));
	pReadFile->read((char*)&m_fMoveSpeed, sizeof(_float));
	pReadFile->read((char*)&m_fFadeInStartTime, sizeof(_float));
	pReadFile->read((char*)&m_fFadeInTime, sizeof(_float));
	pReadFile->read((char*)&m_fFadeOutStartTime, sizeof(_float));
	pReadFile->read((char*)&m_fFadeOutTime, sizeof(_float));
	pReadFile->read((char*)&m_vStartScale, sizeof(_float4));
	pReadFile->read((char*)&m_vFadeInTargetScale, sizeof(_float4));
	pReadFile->read((char*)&m_vFadeOutTargetScale, sizeof(_float4));
	pReadFile->read((char*)&m_fTargetAlpha, sizeof(_float));

	m_eShaderType = SHADER_VTXEFFECTS;

	return S_OK;
}

void CDefault_Effect::My_Tick()
{
	__super::My_Tick();
}

void CDefault_Effect::My_LateTick()
{
	__super::My_LateTick();
}
