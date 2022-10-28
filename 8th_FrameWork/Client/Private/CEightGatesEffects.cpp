#include "stdafx.h"
#include "CEightGatesEffects.h"

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

#include "Functor.h"


CEightGatesEffects::CEightGatesEffects()
{
}

CEightGatesEffects::~CEightGatesEffects()
{
}

CEightGatesEffects* CEightGatesEffects::Create()
{
	CEightGatesEffects* pInstance = new CEightGatesEffects;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CEightGatesEffects");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CEightGatesEffects::OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos)
{
	
}

HRESULT CEightGatesEffects::Start()
{
	CGameObject::Start();

	list<CGameObject*> EffectsList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"EightGatesLoop", this, m_pTransform->Get_MyWorld(WORLD_POS));
	for (auto& elem : EffectsList)
	{
		static_cast<CEffect*>(elem)->Set_EffectFlag(EFFECT_FOLLOWTARGET);
	}

	m_EffectsList.push_back(CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"EightGatesDistortion"), this, m_pTransform->Get_MyWorld(WORLD_POS)));


	LIGHTDESC			LightDesc;

	LightDesc.eType = tagLightDesc::TYPE_POINT;

	LightDesc.fRange = 3.f;

	LightDesc.vDiffuse = _float4(1.f, 0.3f, 0.3f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.pOwner = this;
	LightDesc.vOffset = { 0.f, 0.6f, 0.f, 1.f };

	GAMEINSTANCE->Add_Light(LightDesc);


	return S_OK;
}

void CEightGatesEffects::My_Tick()
{
	m_fCreateAcc += fDT;

	if (m_fCreateAcc > m_fCreateTime)
	{
		CFunctor::Play_Sound(L"Eff_EightGatesLoop", CHANNEL_EFFECTS, m_pTransform->Get_World(WORLD_POS));

		m_fCreateAcc = 0.f;
		list<CGameObject*> EffectsList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"EightGatesLoop", this, m_pTransform->Get_MyWorld(WORLD_POS));
		for (auto& elem : EffectsList)
		{
			static_cast<CEffect*>(elem)->Set_EffectFlag(EFFECT_FOLLOWTARGET);
		}
	}

	m_fParticleAcc += fDT;
	if (m_fParticleAcc > m_fParticleTime)
	{
		m_fParticleAcc = 0.f;
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"EightGatesParticle", this, m_pTransform->Get_MyWorld(WORLD_POS));
		
	}
}

void CEightGatesEffects::My_LateTick()
{
	__super::My_LateTick();

}

void CEightGatesEffects::OnEnable()
{
	list<CGameObject*> EffectsList =  CEffects_Factory::Get_Instance()->Create_MultiEffects(L"EightGatesLoop", this, m_pTransform->Get_MyWorld(WORLD_POS));
	for (auto& elem : EffectsList)
	{
		static_cast<CEffect*>(elem)->Set_EffectFlag(EFFECT_FOLLOWTARGET);
	}

	m_EffectsList.push_back(CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"EightGatesDistortion"), this, m_pTransform->Get_MyWorld(WORLD_POS)));

	m_pCurCell = nullptr;

	m_fCreateAcc = 0.f;

	__super::OnEnable();

	LIGHTDESC			LightDesc;

	LightDesc.eType = tagLightDesc::TYPE_POINT;

	LightDesc.fRange = 3.f;

	LightDesc.vDiffuse = _float4(1.f, 0.3f, 0.3f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.pOwner = this;

	GAMEINSTANCE->Add_Light(LightDesc);
}

void CEightGatesEffects::OnDisable()
{
	CFunctor::Play_Sound(L"Eff_FireBomb", CHANNEL_EFFECTS, m_pTransform->Get_World(WORLD_POS));
	__super::OnDisable();

	DISABLE_GAMEOBJECT(m_EffectsList.back());
	m_EffectsList.clear();

	if (PLAYER->Is_NightGuy())
	{
		_float4 vLook = m_pTransform->Get_World(WORLD_LOOK);
		if (PLAYER->Get_PhysicsCom()->Get_Physics().bAir)
		{
			static_cast<CHit_Ground_Player*>(CState_Manager::Get_Instance()->Get_State(STATE_HIT_GROUND_PLAYER))->On_HitGround(
				CHit_Ground_Player::SPINBLOW, vLook, vLook * -1.f, 13.f, 10.f, 10.f);
		}
		else
		{
			static_cast<CHit_Ground_Player*>(CState_Manager::Get_Instance()->Get_State(STATE_HIT_GROUND_PLAYER))->On_HitGround(
				CHit_Ground_Player::HARDHIT, vLook, vLook * -1.f, 5.f, 0.f, 10.f);
		}

		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"EightGatesStart", PLAYER, PLAYER->Get_Transform()->Get_World(WORLD_POS));
		CUser::Get_Instance()->Get_FollowCam()->Start_ShakingCamera(0.07f, 0.3f);


		CUser::Get_Instance()->On_StyleChange(0);
		PLAYER->Set_Style(CUnit::STYLE_HAND);
		PLAYER->Enter_State(STATE_HIT_GROUND_PLAYER);
	}

}

HRESULT CEightGatesEffects::Initialize_Prototype()
{
	m_eDisableType = FADE;
	m_hcMyCode = HASHCODE(CEightGatesEffects);
	m_bEffectFlag |= EFFECT_FOLLOWTARGET;
	m_fFadeOutStartTime = 20.f;

	return S_OK;
}

HRESULT CEightGatesEffects::Initialize()
{
	return S_OK;
}
