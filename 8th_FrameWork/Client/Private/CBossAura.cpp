#include "stdafx.h"
#include "CBossAura.h"

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
#include "MeshContainer.h"

#include "CHit_Ground.h"
#include "CState_Manager.h"
#include "CScript_FollowCam.h"
#include "CUser.h"
#include "CHit_Ground_Player.h"
#include "CUnit_Player.h"

#include "Texture.h"

#include "CDefault_UI.h"
#include "CGameObject_Factory.h"

#include "CFader.h"


CBossAura::CBossAura()
{
}

CBossAura::~CBossAura()
{
}

CBossAura* CBossAura::Create()
{
	CBossAura* pInstance = new CBossAura;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CBossAura");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CBossAura::OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos)
{

}

void CBossAura::On_StartFog()
{
	m_bFog = true;
	if (!m_pKotoUI)
	{
		m_pKotoUI = CGameObject_Factory::Create_UI(L"KotoAmatsukami");
		CREATE_GAMEOBJECT(m_pKotoUI, GROUP_UI);
	}
	else
		GET_COMPONENT_FROM(m_pKotoUI, CFader)->Force_KeyInput();

	

}

void CBossAura::On_StopFog()
{
	m_bStopFoging = true;
	GET_COMPONENT_FROM(m_pKotoUI, CFader)->Force_KeyInput();

	CUser::Get_Instance()->Enable_HUD();
}

HRESULT CBossAura::Start()
{
	CGameObject::Start();

	list<CGameObject*> EffectsList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"BossAuraLoop", this, m_pTransform->Get_MyWorld(WORLD_POS));
	for (auto& elem : EffectsList)
	{
		static_cast<CEffect*>(elem)->Set_EffectFlag(EFFECT_FOLLOWTARGET);
	}

	LIGHTDESC			LightDesc;

	LightDesc.eType = tagLightDesc::TYPE_POINT;

	LightDesc.fRange = 3.f;

	LightDesc.vDiffuse = _float4(0.f, 0.3f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.pOwner = this;
	LightDesc.vOffset = { 0.f, 0.6f, 0.f, 1.f };

	GAMEINSTANCE->Add_Light(LightDesc);

	return S_OK;
}

void CBossAura::My_Tick()
{
	m_fCreateAcc += fDT;

	if (m_fCreateAcc > m_fCreateTime)
	{
		m_fCreateAcc = 0.f;
		list<CGameObject*> EffectsList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"BossAuraLoop", this, m_pTransform->Get_MyWorld(WORLD_POS));
		for (auto& elem : EffectsList)
		{
			static_cast<CEffect*>(elem)->Set_EffectFlag(EFFECT_FOLLOWTARGET);
		}
	}

	m_fParticleAcc += fDT;
	if (m_fParticleAcc > m_fParticleTime)
	{
		m_fParticleAcc = 0.f;
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"BossAuraParticle", this, m_pTransform->Get_MyWorld(WORLD_POS));

	}
}

void CBossAura::My_LateTick()
{
	Update_FollowTarget();

	m_fCurUVPlusX += m_fUVSpeedX * fDT;
	m_fCurUVPlusY += m_fUVSpeedY * fDT;

	if (m_bFog)
		Update_Fog();

}

void CBossAura::OnEnable()
{
	list<CGameObject*> EffectsList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"BossAuraLoop", this, m_pTransform->Get_MyWorld(WORLD_POS));
	for (auto& elem : EffectsList)
	{
		static_cast<CEffect*>(elem)->Set_EffectFlag(EFFECT_FOLLOWTARGET);
	}


	m_pCurCell = nullptr;

	m_fCreateAcc = 0.f;

	__super::OnEnable();

	LIGHTDESC			LightDesc;

	LightDesc.eType = tagLightDesc::TYPE_POINT;

	LightDesc.fRange = 3.f;

	LightDesc.vDiffuse = _float4(0.f, 0.3f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.pOwner = this;
	LightDesc.vOffset = { 0.f, 0.6f, 0.f, 1.f };

	GAMEINSTANCE->Add_Light(LightDesc);
}

void CBossAura::OnDisable()
{
	__super::OnDisable();

}

void CBossAura::Update_Fog()
{

	CShader* pDeferredShader = GAMEINSTANCE->Get_DeferredShader();

	if (m_bStopFoging)
	{
		m_fFogAlpha -= fDT;

		if (m_fFogAlpha < 0.f)
		{
			m_fFogAlpha = 0.f;
			m_bFog = false;
			m_bStopFoging = false;
			DISABLE_GAMEOBJECT(this);
			return;
		}
	}
	else
	{
		m_fFogAlpha += 1.f * fDT;

		if (m_fFogAlpha > 1.f)
			m_fFogAlpha = 1.f;
	}

	

	GET_COMPONENT(CTexture)->Set_ShaderResourceView(pDeferredShader, "g_FogTexture");

	pDeferredShader->Set_RawValue("g_fUVPlusX", &m_fCurUVPlusX, sizeof(_float));
	pDeferredShader->Set_RawValue("g_fUVPlusY", &m_fCurUVPlusY, sizeof(_float));
	pDeferredShader->Set_RawValue("g_fFogAlpha", &m_fFogAlpha, sizeof(_float));
}

HRESULT CBossAura::Initialize_Prototype()
{
	m_eDisableType = FADE;
	m_hcMyCode = HASHCODE(CBossAura);
	m_bEffectFlag |= EFFECT_FOLLOWTARGET;
	m_fFadeOutStartTime = 9999.f;

	m_fUVSpeedX = -0.05f;
	m_fUVSpeedY = 0.0f;


	CTexture* pTexture = CTexture::Create(0, L"../bin/resources/Textures/effects/noise/T_EFF_Noise_16_M.dds", 1);
	Add_Component(pTexture);


	return S_OK;
}

HRESULT CBossAura::Initialize()
{
	return S_OK;
}
