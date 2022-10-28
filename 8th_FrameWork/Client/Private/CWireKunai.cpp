#include "stdafx.h"
#include "CWireKunai.h"

#include "HIerarchyNode.h"
#include "GameInstance.h"

#include "Model.h"
#include "Physics.h"
#include "CUnit.h"
#include "CModel_Renderer.h"
#include "CShader.h"

#include "Functor.h"
#include "Transform.h"
#include "CComponent_Factory.h"

CWireKunai::CWireKunai()
{
}

CWireKunai::~CWireKunai()
{
}

CWireKunai* CWireKunai::Create()
{
	CWireKunai* pInstance = new CWireKunai;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CWireKunai");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CWireKunai::Start()
{
	__super::Start();
	_float4 vDir = m_vTargetPosition - m_pTransform->Get_MyWorld(WORLD_POS);
	GET_COMPONENT(CPhysics)->Set_Dir(vDir);
	GET_COMPONENT(CPhysics)->Set_SpeedasMax();
	m_pTransform->Set_Look(vDir);

	return S_OK;
}

void CWireKunai::OnEnable()
{
	m_bSound = false;

	_float4 vDir = m_vTargetPosition - m_pTransform->Get_MyWorld(WORLD_POS);
	GET_COMPONENT(CPhysics)->Set_Dir(vDir);
	GET_COMPONENT(CPhysics)->Set_SpeedasMax();
	m_pTransform->Set_Look(vDir);
}

void CWireKunai::My_Tick()
{
	__super::My_Tick();

	if (GET_COMPONENT(CPhysics)->Get_Physics().fSpeed <= 0.f)
	{
		if (!m_bSound)
		{
			CFunctor::Play_Sound(L"Eff_WireHit", CHANNEL_EFFECTS, m_pTransform->Get_World(WORLD_POS));
			m_bSound = true;
		}
	}

}

HRESULT CWireKunai::Initialize_Prototype()
{
	m_eDisableType = FADE;
	m_fFadeOutStartTime = 2.f;
	m_wstrPath = L"../bin/resources/meshes/weapons/SM_WEP_Kunai_basic.fbx";
	m_matTrans = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(-90.0f));
	m_hcMyCode = HASHCODE(CWireKunai);
	m_iPassType = VTXEFFECT_PASS_DEFAULT;
	m_fMoveSpeed = 90.f;
	m_bEffectFlag = EFFECT_TARGETPOS;


	Add_Component(CComponent_Factory::Clone_Component<CPhysics>(this));
	GET_COMPONENT(CPhysics)->Set_Dir(_float4(0.f, 0.f, 0.f, 0.f));
	GET_COMPONENT(CPhysics)->Set_MaxSpeed(m_fMoveSpeed);


	return __super::Initialize_Prototype();
}