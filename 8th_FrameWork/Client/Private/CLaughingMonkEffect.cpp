#include "stdafx.h"
#include "CLaughingMonkEffect.h"

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
#include "CAnimator.h"

#include "CHit_Ground.h"
#include "CState_Manager.h"
#include "CScript_FollowCam.h"
#include "CUser.h"

#include "Functor.h"

CLaughingMonkEffect::CLaughingMonkEffect()
{
}

CLaughingMonkEffect::~CLaughingMonkEffect()
{
}

CLaughingMonkEffect* CLaughingMonkEffect::Create()
{
	CLaughingMonkEffect* pInstance = new CLaughingMonkEffect;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CLaughingMonkEffect");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CLaughingMonkEffect::Start()
{
	CGameObject::Start();

	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"BigSmoky", this, m_pTransform->Get_MyWorld(WORLD_POS));
	CEffects_Factory::Get_Instance()->Create_BigLandingEffects(m_pTransform->Get_World(WORLD_POS), m_pCurCell);


	return S_OK;
}

void CLaughingMonkEffect::My_Tick()
{
	if (m_bFinished)
	{
		if (GET_COMPONENT_FROM(m_EffectsList.front(), CAnimator)->Is_CurAnimFinished())
		{
			DISABLE_GAMEOBJECT(this);
		}

		return;
	}



	m_fDelayAcc += fDT;

	if (!m_bLittleDelay)
	{
		if (m_fDelayAcc > 0.2f)
		{
			m_bLittleDelay = true;
			m_EffectsList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"WoodHands", this, m_pTransform->Get_MyWorld(WORLD_POS));
			
			CGameObject* pGroundBreak = CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"GroundBreak_C"), m_EffectsList.front()->Get_Transform()->Get_World(WORLD_POS));
			pGroundBreak->Get_Transform()->Set_Up(_float4(0.f, 1.f, 0.f, 0.f));

			auto iter = m_EffectsList.begin();
			++iter;

			pGroundBreak = CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"GroundBreak_C"), (*iter)->Get_Transform()->Get_World(WORLD_POS));
			pGroundBreak->Get_Transform()->Set_Up(_float4(0.f, 1.f, 0.f, 0.f));
			
			
			m_fDelayAcc = 0.f;
		}

		return;
	}


	//Attack
	if (!m_bOnce)
	{
		if (m_fDelayAcc > m_fDelayTime)
		{
			CFunctor::Play_Sound(L"Eff_FireBomb", CHANNEL_EFFECTS, m_pTransform->Get_World(WORLD_POS));

			CUser::Get_Instance()->Get_FollowCam()->Start_ShakingCamera(0.08f, 0.4f);
			m_bOnce = true;
			m_fDelayAcc = 0.f;
			list<CGameObject*> listEffect = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"LaughingMonkHit", this, m_pTransform->Get_MyWorld(WORLD_POS));
			static_cast<CEffect*>(listEffect.back())->Set_ColliderOn(4.f, m_eColType);
		}
		
	}
	else if (m_fDelayAcc > 0.3f)
	{
		//0.3f after Attack
		m_bFinished = true;
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"BigSmoky", this, m_pTransform->Get_MyWorld(WORLD_POS));
		CEffects_Factory::Get_Instance()->Create_BigLandingEffects(m_pTransform->Get_World(WORLD_POS), m_pCurCell);


		CFunctor::Play_Sound(L"Eff_Summon", CHANNEL_EFFECTS, m_pTransform->Get_World(WORLD_POS));
		//m_fDelayAcc = 0.f;
	}

	
}

void CLaughingMonkEffect::My_LateTick()
{
	//__super::My_LateTick();
}

void CLaughingMonkEffect::OnEnable()
{
	

	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"BigSmoky", this, m_pTransform->Get_MyWorld(WORLD_POS));


	CEffects_Factory::Get_Instance()->Create_BigLandingEffects(m_pTransform->Get_World(WORLD_POS), m_pCurCell);

	

	m_bLittleDelay = false;
	m_bOnce = false;
	m_bFinished = false;
	m_fDelayAcc = 0.f;
	__super::OnEnable();
}

void CLaughingMonkEffect::OnDisable()
{
	__super::OnDisable();

	for (auto& elem : m_EffectsList)
		DISABLE_GAMEOBJECT(elem);



}

HRESULT CLaughingMonkEffect::Initialize_Prototype()
{
	m_eDisableType = NONE;
	m_hcMyCode = HASHCODE(CLaughingMonkEffect);

	return S_OK;
}

HRESULT CLaughingMonkEffect::Initialize()
{

	return S_OK;
}
