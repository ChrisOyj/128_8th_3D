#include "stdafx.h"
#include "CBoneChakra.h"

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

#include "CEffects_Factory.h"

CBoneChakra::CBoneChakra()
{
}

CBoneChakra::~CBoneChakra()
{
}

CBoneChakra* CBoneChakra::Create()
{
	CBoneChakra* pInstance = new CBoneChakra;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CBoneChakra");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CBoneChakra::Start()
{

	m_pEffect = CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"BoneChakra"), this);


	return S_OK;
}

void CBoneChakra::My_Tick()
{
	__super::My_Tick();
}

void CBoneChakra::My_LateTick()
{
	__super::My_LateTick();
}

void CBoneChakra::OnEnable()
{
	ENABLE_GAMEOBJECT(m_pEffect);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pTrailEffect, CMesh))->Set_TrailOn();

	__super::OnEnable();
}

void CBoneChakra::OnDisable()
{
	__super::OnDisable();

	DISABLE_GAMEOBJECT(m_pEffect);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pTrailEffect, CMesh))->Set_TrailOff();

}

HRESULT CBoneChakra::Initialize_Prototype()
{
	m_eDisableType = FADE;

	m_fFadeOutStartTime = 9999.f;

	m_hcMyCode = HASHCODE(CBoneChakra);

	m_bEffectFlag = EFFECT_REFBONE;




	return S_OK;
}

HRESULT CBoneChakra::Initialize()
{

	m_pTrailEffect = CTrailEffect::Create(CP_RIGHTBEFORE_RENDERER, 60, _float4(0.03f, 0.f, 0.03f, 1.f), _float4(-0.03f, 0.f, -0.03f, 1.f)
		, nullptr, m_pTransform, _float4(0.f, 0.6f, 1.0f, 0.7f), _float4(1.f, 1.f, 1.f, 3.f),
		L"../bin/resources/textures/effects/gradientMap/T_EFF_Blur_04_M.dds",
		L"../bin/resources/textures/effects/gradationcolor/T_EFF_GMS_GuardianArmor_01_02_BC.png"
	);


	m_pTrailEffect->Initialize();

	m_pTrailEffect->Set_EffectFlag(SH_EFFECT_DEFAULT);

	GET_COMPONENT_FROM(m_pTrailEffect, CRenderer)->Set_Pass(VTXTEX_PASS_FOOTTRAIL);

	CREATE_GAMEOBJECT(m_pTrailEffect, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pTrailEffect, CMesh))->Set_TrailOn();
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pTrailEffect, CMesh))->Set_NoCurve();


	if (!m_pTrailEffect)
		return E_FAIL;

	return S_OK;
}
