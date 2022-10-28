#include "stdafx.h"
#include "CKamuiEffect.h"

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

#include "Functor.h"


CKamuiEffect::CKamuiEffect()
{
}

CKamuiEffect::~CKamuiEffect()
{
}

CKamuiEffect* CKamuiEffect::Create()
{
	CKamuiEffect* pInstance = new CKamuiEffect;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CKamuiEffect");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CKamuiEffect::OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos)
{
	if (m_bOnce)
	{
			ENABLE_COMPONENT(GET_COMPONENT_FROM(pOtherObj, CRenderer));

	}

	if (m_bDisAppear && !m_bOnce)
	{
		DISABLE_COMPONENT(GET_COMPONENT_FROM(pOtherObj, CRenderer));
		m_EnemyList.push_back(pOtherObj);
	}
}

HRESULT CKamuiEffect::Start()
{
	CGameObject::Start();

	m_EffectsList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"KamuiAttack", this, m_pTransform->Get_MyWorld(WORLD_POS));

	CallBack_CollisionEnter += bind(&CKamuiEffect::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3);
	static_cast<CHit_Ground*>(CState_Manager::Get_Instance()->Get_State(STATE_HIT_GROUND_ENEMY))->On_HitGround(
		CHit_Ground::HEAD, 1.5f, 1.5f, 0.1f);

	return S_OK;
}

void CKamuiEffect::My_Tick()
{
	m_fBoomAcc += fDT;

	if (!m_bOnce && m_fBoomAcc > m_fBomTime)
	{
		CFunctor::Play_Sound(L"Eff_Hit_Hard", CHANNEL_EFFECTS, m_pTransform->Get_World(WORLD_POS));


		m_fBoomAcc = 0.f;
		CEffects_Factory::Get_Instance()->Create_ScreenEffects(this);
		m_bOnce = true;

		//펑 터지는거 활성화
	
		for (auto& elem : m_EnemyList)
		{
			ENABLE_COMPONENT(GET_COMPONENT_FROM(elem, CRenderer));
		}

		CCollider_Sphere* pCol = GET_COMPONENT(CCollider_Sphere);

		ENABLE_COMPONENT(pCol);


		CUser::Get_Instance()->Get_FollowCam()->Start_ShakingCamera(0.09f, 0.3f);

		static_cast<CHit_Ground*>(CState_Manager::Get_Instance()->Get_State(STATE_HIT_GROUND_ENEMY))->On_HitGround(
			CHit_Ground::SPINBLOW, 13.f, 10.f, 50.f);

		if (m_pCurCell)
		{
			CEffects_Factory::Get_Instance()->Create_BigLandingEffects(m_pTransform->Get_World(WORLD_POS), m_pCurCell);
		}

		return;
	}


	//껏다가, 켰다가, 다시 꺼야함 ㅋ;

	if (m_bDisAppear)
	{
		m_fColAcc += fDT;

		if (m_fColAcc > m_fColTime)
		{
			CCollider_Sphere* pCol = GET_COMPONENT(CCollider_Sphere);

			if (pCol->Is_Disable())
			{
				static_cast<CHit_Ground*>(CState_Manager::Get_Instance()->Get_State(STATE_HIT_GROUND_ENEMY))->On_HitGround(
					CHit_Ground::HEAD, 1.5f, 1.5f, 0.1f);

				ENABLE_COMPONENT(pCol);
				m_iDisappearCnt++;
			}
			else
			{
				DISABLE_COMPONENT(pCol);

				//한번 켜졌다가 온거면 무조건 끝
				if (m_iDisappearCnt > 0)
				{
					m_fColTime = 9999.f;
				}
			}


		}

	}

	if (!m_bDisAppear && m_fBoomAcc > m_fDisappearTime)
	{
		CCollider_Sphere* pCol = GET_COMPONENT(CCollider_Sphere);
		static_cast<CHit_Ground*>(CState_Manager::Get_Instance()->Get_State(STATE_HIT_GROUND_ENEMY))->On_HitGround(
			CHit_Ground::HEAD, 1.5f, 1.5f, 0.1f);
		ENABLE_COMPONENT(pCol);

		m_bDisAppear = true;
		m_fColAcc = 0.f;
		return;
	}

	m_fColAcc += fDT;

	if (!m_bDisAppear && m_fColAcc > m_fColTime)
	{
		CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"KamuiText"), m_pTransform->Get_World(WORLD_POS));


		CCollider_Sphere* pCol = GET_COMPONENT(CCollider_Sphere);
		if (pCol->Is_Disable())
		{
			static_cast<CHit_Ground*>(CState_Manager::Get_Instance()->Get_State(STATE_HIT_GROUND_ENEMY))->On_HitGround(
				CHit_Ground::HEAD, 1.5f, 1.5f, 0.1f);

			ENABLE_COMPONENT(pCol);
		}
		else
			DISABLE_COMPONENT(pCol);

		m_fColAcc = 0.f;
	}



}

void CKamuiEffect::My_LateTick()
{
	if (m_bOnce)
	{
		if (m_EffectsList.back()->Is_Disable())
			DISABLE_GAMEOBJECT(this);
	}
	
}

void CKamuiEffect::OnEnable()
{
	m_EffectsList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"KamuiAttack", this, m_pTransform->Get_MyWorld(WORLD_POS));

	/*if (m_pCurCell)
	{
		CEffects_Factory::Get_Instance()->Create_BigLandingEffects(m_pTransform->Get_World(WORLD_POS), m_pCurCell);
	}*/

	static_cast<CHit_Ground*>(CState_Manager::Get_Instance()->Get_State(STATE_HIT_GROUND_ENEMY))->On_HitGround(
		CHit_Ground::HEAD, 1.5f, 1.5f, 0.1f);

	m_pCurCell = nullptr;

	m_EnemyList.clear();
	m_fBoomAcc = 0.f;
	m_fColAcc = 0.f;
	m_fColTime = 0.07f;
	m_bDisAppear = false;
	m_bOnce = false;
	m_iDisappearCnt = 0;


	__super::OnEnable();

	DISABLE_COMPONENT(GET_COMPONENT(CCollider_Sphere));

}

void CKamuiEffect::OnDisable()
{
	__super::OnDisable();


	
}

HRESULT CKamuiEffect::Initialize_Prototype()
{
	m_eDisableType = NONE;
	m_hcMyCode = HASHCODE(CKamuiEffect);

	CCollider_Sphere* pCollider = CComponent_Factory::Clone_Component<CCollider_Sphere>(this);
	pCollider->Get_ColInfo().fRadius = 3.f;
	pCollider->Set_ColIndex(COL_PLAYERSKILL);
	Add_Component(pCollider);


	return S_OK;
}

HRESULT CKamuiEffect::Initialize()
{
	GET_COMPONENT(CCollider_Sphere)->Initialize();
	GET_COMPONENT(CCollider_Sphere)->Set_ColIndex(COL_PLAYERSKILL);
	DISABLE_COMPONENT(GET_COMPONENT(CCollider_Sphere));

	/*if (m_pCurCell)
	{
		CEffects_Factory::Get_Instance()->Create_BigLandingEffects(m_pTransform->Get_World(WORLD_POS), m_pCurCell);
	}*/

	return S_OK;
}
