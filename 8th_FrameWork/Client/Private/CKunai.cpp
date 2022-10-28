#include "stdafx.h"
#include "CKunai.h"

#include "HIerarchyNode.h"
#include "GameInstance.h"

#include "Model.h"
#include "Physics.h"
#include "CUnit.h"
#include "CModel_Renderer.h"
#include "CShader.h"

#include "CCollider_Sphere.h"
#include "Transform.h"
#include "CComponent_Factory.h"
#include "CNavigation.h"

#include "CTrailEffect.h"
#include "CTrailBuffer.h"

CKunai::CKunai()
{
}

CKunai::~CKunai()
{
}

CKunai* CKunai::Create()
{
	CKunai* pInstance = new CKunai;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CKunai");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CKunai::Start()
{
	__super::Start();
	GET_COMPONENT(CPhysics)->Set_NaviOn();

	_float4 vDir = m_vTargetPosition - m_pTransform->Get_MyWorld(WORLD_POS);
	GET_COMPONENT(CPhysics)->Set_Dir(vDir);
	m_pTransform->Set_Look(vDir);

	GET_COMPONENT(CPhysics)->Set_SpeedasMax();

	

	return S_OK;
}

void CKunai::My_Tick()
{
	__super::My_Tick();
	GET_COMPONENT(CPhysics)->Get_Physics().bAir = true;
}

void CKunai::My_LateTick()
{
	//벽에 박히면 사라지게
	if (GET_COMPONENT(CNavigation)->Is_OnWall())
	{
		DISABLE_GAMEOBJECT(this);
		return;
	}


	__super::My_LateTick();
}

void CKunai::OnEnable()
{
	_float4 vDir = m_vTargetPosition - m_pTransform->Get_MyWorld(WORLD_POS);
	GET_COMPONENT(CPhysics)->Set_Dir(vDir);
	m_pTransform->Set_Look(vDir);
	GET_COMPONENT(CNavigation)->Reset_OnWall();
	ENABLE_GAMEOBJECT(m_pTrailEffect);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pTrailEffect, CMesh))->Set_TrailOn();
	GET_COMPONENT(CPhysics)->Set_SpeedasMax();

	__super::OnEnable();
}

void CKunai::OnDisable()
{
	__super::OnDisable();
	DISABLE_GAMEOBJECT(m_pTrailEffect);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pTrailEffect, CMesh))->Set_TrailOff();

}

HRESULT CKunai::Initialize_Prototype()
{
	m_eDisableType = FADE;

	m_fFadeOutStartTime = 2.f;

	m_wstrPath = L"../bin/resources/meshes/weapons/SM_WEP_Kunai_basic.fbx";
	m_matTrans = XMMatrixScaling(0.02f, 0.02f, 0.02f) * XMMatrixRotationY(XMConvertToRadians(-90.0f));
	m_hcMyCode = HASHCODE(CKunai);
	m_iPassType = VTXEFFECT_PASS_TEXT;
	m_fMoveSpeed = 50.f;
	m_bEffectFlag = EFFECT_TARGETPOS;
	m_vEffectFlag = SH_EFFECT_DEFAULT;


	Add_Component(CComponent_Factory::Clone_Component<CPhysics>(this));
	GET_COMPONENT(CPhysics)->Set_Dir(_float4(0.f, 0.f, 0.f, 0.f));
	GET_COMPONENT(CPhysics)->Set_MaxSpeed(m_fMoveSpeed);
	GET_COMPONENT(CPhysics)->Set_Speed(m_fMoveSpeed);
	GET_COMPONENT(CPhysics)->Get_Physics().fGravity = 0.f;
	GET_COMPONENT(CPhysics)->Get_Physics().bAir = true;
	CCollider_Sphere* pCollider = CComponent_Factory::Clone_Component<CCollider_Sphere>(this);
	pCollider->Get_ColInfo().fRadius = 0.25f;
	pCollider->Set_ColIndex(COL_PLAYERTHROW);
	Add_Component(pCollider);

	
	CNavigation* pNaviCom = CNavigation::Create(CP_NAVIGATION, nullptr, GET_COMPONENT(CPhysics));

	if (!pNaviCom)
		return E_FAIL;

	Add_Component(pNaviCom);

	return __super::Initialize_Prototype();
}

HRESULT CKunai::Initialize()
{
	GET_COMPONENT(CCollider_Sphere)->Initialize();
	GET_COMPONENT(CCollider_Sphere)->Set_ColIndex(COL_PLAYERTHROW);

	m_pTrailEffect = CTrailEffect::Create(CP_RIGHTBEFORE_RENDERER, 14, _float4(0.03f, 0.f, 0.f, 1.f), _float4(-0.03f, 0.f, 0.f, 1.f)
		, nullptr, m_pTransform, _float4(0.f, 0.5f, 1.0f, 0.5f), _float4(1.f, 1.f, 1.f, 3.f),
		L"../bin/resources/textures/effects/gradientMap/T_EFF_Blur_04_M.dds",
		L"../bin/resources/textures/effects/gradationcolor/T_EFF_GMS_Light_01_BC.png"
	);
	m_pTrailEffect->Initialize();
	CREATE_GAMEOBJECT(m_pTrailEffect, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pTrailEffect, CMesh))->Set_TrailOn();
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pTrailEffect, CMesh))->Set_NoCurve();


	if (!m_pTrailEffect)
		return E_FAIL;

	return S_OK;
}
