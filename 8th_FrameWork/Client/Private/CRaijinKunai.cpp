#include "stdafx.h"
#include "CRaijinKunai.h"

#include "HIerarchyNode.h"
#include "GameInstance.h"

#include "Model.h"
#include "Physics.h"
#include "CUnit.h"
#include "CModel_Renderer.h"
#include "CShader.h"


#include "Transform.h"
#include "CComponent_Factory.h"

#include "CTrailEffect.h"
#include "CTrailBuffer.h"
#include "Functor.h"
CRaijinKunai::CRaijinKunai()
{
}

CRaijinKunai::~CRaijinKunai()
{
}

CRaijinKunai* CRaijinKunai::Create()
{
	CRaijinKunai* pInstance = new CRaijinKunai;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CRaijinKunai");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CRaijinKunai::Start()
{
	__super::Start();
	_float4 vDir = m_vTargetPosition - m_pTransform->Get_MyWorld(WORLD_POS);
	GET_COMPONENT(CPhysics)->Set_Dir(vDir);
	GET_COMPONENT(CPhysics)->Set_SpeedasMax();
	m_pTransform->Set_Look(vDir);

	return S_OK;
}

void CRaijinKunai::My_LateTick()
{
	if (GET_COMPONENT(CPhysics)->Get_Physics().fSpeed <= 0.f)
	{
		if (!m_bSound)
		{
			CFunctor::Play_Sound(L"Eff_WireHit", CHANNEL_EFFECTS, m_pTransform->Get_World(WORLD_POS));
			m_bSound = true;
		}
	}

	__super::My_LateTick();

	m_fAlpha = 1.f;
}

void CRaijinKunai::OnEnable()
{
	_float4 vDir = m_vTargetPosition - m_pTransform->Get_MyWorld(WORLD_POS);
	GET_COMPONENT(CPhysics)->Set_Dir(vDir);
	GET_COMPONENT(CPhysics)->Set_SpeedasMax();
	m_pTransform->Set_Look(vDir);


	ENABLE_GAMEOBJECT(m_pTrailEffect);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pTrailEffect, CMesh))->Set_TrailOn();

	if (m_bSuccess)
		m_fFadeOutTime = 2.5f;
	else
		m_fFadeOutTime = 0.f;

}

void CRaijinKunai::OnDisable()
{
	__super::OnDisable();
	m_bSuccess = false;
	m_bSound = false;

	DISABLE_GAMEOBJECT(m_pTrailEffect);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pTrailEffect, CMesh))->Set_TrailOff();
}

HRESULT CRaijinKunai::Initialize_Prototype()
{
	m_eDisableType = FADE;
	m_fFadeOutStartTime = 2.f;
	m_fFadeOutTime = 3.f;
	m_wstrPath = L"../bin/resources/meshes/weapons/Throwing/SM_WEP_MinatoKunai.fbx";
	m_matTrans = XMMatrixScaling(0.01f, 0.01f, 0.01f) *XMMatrixRotationY(XMConvertToRadians(180.0f));
	m_hcMyCode = HASHCODE(CRaijinKunai);
	m_iPassType = VTXEFFECT_PASS_DEFAULT;
	m_fMoveSpeed = 90.f;
	m_bEffectFlag = EFFECT_TARGETPOS;

	m_vEffectFlag = SH_EFFECT_NOBLOOM;
	m_vGlowFlag = _float4(1.f, 0.9f, 0.3f, 0.7f);

	Add_Component(CComponent_Factory::Clone_Component<CPhysics>(this));
	GET_COMPONENT(CPhysics)->Set_Dir(_float4(0.f, 0.f, 0.f, 0.f));
	GET_COMPONENT(CPhysics)->Set_MaxSpeed(m_fMoveSpeed);




	return __super::Initialize_Prototype();
}

HRESULT CRaijinKunai::Initialize()
{
	m_pTrailEffect = CTrailEffect::Create(CP_RIGHTBEFORE_RENDERER, 10, _float4(0.03f, 0.f, 0.f, 1.f), _float4(-0.03f, 0.f, 0.f, 1.f)
		, nullptr, m_pTransform, _float4(1.f, 0.9f, 0.3f, 0.5f), _float4(1.f, 1.f, 1.f, 3.f),
		L"../bin/resources/textures/effects/gradientMap/T_EFF_Blur_04_M.dds",
		L"../bin/resources/textures/effects/gradationcolor/T_EFF_GMS_Light_01_BC.png"
	);
	m_pTrailEffect->Initialize();
	CREATE_GAMEOBJECT(m_pTrailEffect, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pTrailEffect, CMesh))->Set_TrailOn();
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pTrailEffect, CMesh))->Set_NoCurve();

	if (m_bSuccess)
		m_fFadeOutTime = 2.5f;
	else
		m_fFadeOutTime = 0.f;

	if (!m_pTrailEffect)
		return E_FAIL;

	return S_OK;
}
