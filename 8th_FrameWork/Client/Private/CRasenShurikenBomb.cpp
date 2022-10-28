#include "stdafx.h"
#include "CRasenShurikenBomb.h"

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

CRasenShurikenBomb::CRasenShurikenBomb()
{
}

CRasenShurikenBomb::~CRasenShurikenBomb()
{
}

CRasenShurikenBomb* CRasenShurikenBomb::Create()
{
	CRasenShurikenBomb* pInstance = new CRasenShurikenBomb;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CRasenShurikenBomb");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CRasenShurikenBomb::OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos)
{
}

HRESULT CRasenShurikenBomb::Start()
{
	CGameObject::Start();

	m_EffectsList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"RasenShuriken_Attack", this, m_pTransform->Get_MyWorld(WORLD_POS));
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"RasenShuriken_Particle", this, m_pTransform->Get_MyWorld(WORLD_POS));

	CallBack_CollisionEnter += bind(&CRasenShurikenBomb::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3);
	CEffects_Factory::Get_Instance()->Create_ScreenEffects(this);

	static_cast<CHit_Ground*>(CState_Manager::Get_Instance()->Get_State(STATE_HIT_GROUND_ENEMY))->On_HitGround(
		CHit_Ground::HEAD, 1.5f, 1.5f, 0.5f);



	LIGHTDESC			LightDesc;

	LightDesc.eType = tagLightDesc::TYPE_POINT;

	LightDesc.fRange = 15.f;

	LightDesc.vDiffuse = _float4(0.1f, 0.5f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.3f, 0.3f, 0.3f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.pOwner = this;

	GAMEINSTANCE->Add_Light(LightDesc);

	return S_OK;
}

void CRasenShurikenBomb::My_Tick()
{
	m_fBoomAcc += fDT;

	if (m_fBoomAcc > m_fBomTime)
	{
		m_bBombed = true;
		m_fBoomAcc = 0.f;
		CFunctor::Play_Sound(L"Eff_RasenShuriken_Boom", CHANNEL_EFFECTS, m_pTransform->Get_World(WORLD_POS));
		CFunctor::Play_Sound(L"Eff_Hit_Hard", CHANNEL_EFFECTS, m_pTransform->Get_World(WORLD_POS));

		list<CGameObject*>	EffectsList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"RasenShuriken_Bomb", this, m_pTransform->Get_MyWorld(WORLD_POS));
		static_cast<CEffect*>(EffectsList.front())->Set_ColliderOn(7.5f, COL_PLAYERSKILL);

		CUser::Get_Instance()->Get_FollowCam()->Start_ShakingCamera(0.09f, 0.3f);

		static_cast<CHit_Ground*>(CState_Manager::Get_Instance()->Get_State(STATE_HIT_GROUND_ENEMY))->On_HitGround(
			CHit_Ground::SPINBLOW, 13.f, 10.f, 30.f);

		if (m_pCurCell)
		{
			CEffects_Factory::Get_Instance()->Create_BigLandingEffects(m_pTransform->Get_World(WORLD_POS), m_pCurCell);
		}

		CCollider_Sphere* pCol = GET_COMPONENT(CCollider_Sphere);
		DISABLE_COMPONENT(pCol);


		return;
	}

	m_fColAcc += fDT;

	if (!m_bBombed && m_fColAcc > m_fColTime)
	{
		CCollider_Sphere* pCol = GET_COMPONENT(CCollider_Sphere);
		if (pCol->Is_Disable())
		{
			CFunctor::Play_Sound(L"Eff_RasenShuriken_Loop", CHANNEL_EFFECTS, m_pTransform->Get_World(WORLD_POS));

			static_cast<CHit_Ground*>(CState_Manager::Get_Instance()->Get_State(STATE_HIT_GROUND_ENEMY))->On_HitGround(
				CHit_Ground::HEAD, 1.5f, 1.5f, 0.5f);

			ENABLE_COMPONENT(pCol);
		}
		else
			DISABLE_COMPONENT(pCol);

		m_fColAcc = 0.f;
	}
}

void CRasenShurikenBomb::My_LateTick()
{
	if (m_bBombed && m_EffectsList.front()->Is_Disable())
		DISABLE_GAMEOBJECT(this);
}

void CRasenShurikenBomb::OnEnable()
{
	m_EffectsList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"RasenShuriken_Attack", this, m_pTransform->Get_MyWorld(WORLD_POS));
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"RasenShuriken_Particle", this, m_pTransform->Get_MyWorld(WORLD_POS));

	m_fBoomAcc = 0.f;

	if (m_pCurCell)
	{
		CEffects_Factory::Get_Instance()->Create_BigLandingEffects(m_pTransform->Get_World(WORLD_POS), m_pCurCell);
	}
	CEffects_Factory::Get_Instance()->Create_ScreenEffects(this);

	static_cast<CHit_Ground*>(CState_Manager::Get_Instance()->Get_State(STATE_HIT_GROUND_ENEMY))->On_HitGround(
		CHit_Ground::HEAD, 1.5f, 1.5f, 0.5f);

	__super::OnEnable();

	LIGHTDESC			LightDesc;

	LightDesc.eType = tagLightDesc::TYPE_POINT;

	LightDesc.fRange = 15.f;

	LightDesc.vDiffuse = _float4(0.1f, 0.5f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.3f, 0.3f, 0.3f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.pOwner = this;

	GAMEINSTANCE->Add_Light(LightDesc);
}

void CRasenShurikenBomb::OnDisable()
{
	__super::OnDisable();

	for (auto& elem : m_EffectsList)
		DISABLE_GAMEOBJECT(elem);

	m_pCurCell = nullptr;
	m_bBombed = false;


}

HRESULT CRasenShurikenBomb::Initialize_Prototype()
{
	m_eDisableType = NONE;
	m_hcMyCode = HASHCODE(CRasenShurikenBomb);

	CCollider_Sphere* pCollider = CComponent_Factory::Clone_Component<CCollider_Sphere>(this);
	pCollider->Get_ColInfo().fRadius = 6.f;
	pCollider->Set_ColIndex(COL_PLAYERSKILL);
	Add_Component(pCollider);


	return S_OK;
}

HRESULT CRasenShurikenBomb::Initialize()
{
	GET_COMPONENT(CCollider_Sphere)->Initialize();
	GET_COMPONENT(CCollider_Sphere)->Set_ColIndex(COL_PLAYERSKILL);

	if (m_pCurCell)
	{
		CEffects_Factory::Get_Instance()->Create_BigLandingEffects(m_pTransform->Get_World(WORLD_POS), m_pCurCell);
	}

	return S_OK;
}
