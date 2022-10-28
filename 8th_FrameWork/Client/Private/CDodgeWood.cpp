#include "stdafx.h"
#include "CDodgeWood.h"

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

#include "CUtility_Transform.h"

CDodgeWood::CDodgeWood()
{
}

CDodgeWood::~CDodgeWood()
{
}

CDodgeWood* CDodgeWood::Create()
{
	CDodgeWood* pInstance = new CDodgeWood;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CDodgeWood");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}


HRESULT CDodgeWood::Initialize_Prototype()
{
	m_eShaderType = SHADER_VTXMODEL;
	m_eDisableType = FADE;
	m_fFadeOutStartTime = 1.5f;
	m_wstrPath = L"../bin/resources/meshes/effects/dodge/SM_EFF_Tree_01.fbx";
	m_matTrans = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	m_hcMyCode = HASHCODE(CDodgeWood);
	m_iPassType = VTXMODEL_PASS_DEFAULT;
	m_vEffectFlag = SH_LIGHT_NOSPEC;


	Add_Component(CComponent_Factory::Clone_Component<CPhysics>(this));
	GET_COMPONENT(CPhysics)->Set_Dir(_float4(0.f, 0.f, 0.f, 0.f));


	return __super::Initialize_Prototype();
}

HRESULT CDodgeWood::Start()
{
	__super::Start();
	GET_COMPONENT(CPhysics)->Set_Speed(10.f);
	GET_COMPONENT(CPhysics)->Set_TurnDir(m_pTransform->Get_World(WORLD_LOOK));
	GET_COMPONENT(CPhysics)->Set_TurnSpeed(0.5f);
	GET_COMPONENT(CPhysics)->Set_Jump(4.f);
	GET_COMPONENT(CPhysics)->Get_PhysicsDetail().fCurGroundY = -10.f;
	GET_COMPONENT(CRenderer)->Set_RenderGroup(RENDER_NONALPHA);

	return S_OK;
}

void CDodgeWood::OnEnable()
{
	__super::OnEnable();
	GET_COMPONENT(CPhysics)->Set_Speed(10.f);
	GET_COMPONENT(CPhysics)->Set_TurnDir(m_pTransform->Get_World(WORLD_LOOK));
	GET_COMPONENT(CPhysics)->Set_TurnSpeed(0.5f);
	GET_COMPONENT(CPhysics)->Set_Jump(4.f);
	GET_COMPONENT(CPhysics)->Get_PhysicsDetail().fCurGroundY = -10.f;
}
