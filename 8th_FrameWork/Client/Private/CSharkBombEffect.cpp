#include "stdafx.h"
#include "CSharkBombEffect.h"

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

#include "CHit_Ground_Player.h"
#include "CUser.h"

#include "Functor.h"

CSharkBombEffect::CSharkBombEffect()
{
}

CSharkBombEffect::~CSharkBombEffect()
{
}

CSharkBombEffect* CSharkBombEffect::Create()
{
	CSharkBombEffect* pInstance = new CSharkBombEffect;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CSharkBombEffect");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CSharkBombEffect::OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos)
{
	//DISABLE_GAMEOBJECT(this);
	

	if (eColType == COL_ENEMYHITBOX)
	{
		CFunctor::Play_Sound(L"Eff_Hit_Hard", CHANNEL_EFFECTS, m_pTransform->Get_World(WORLD_POS));
		CUser::Get_Instance()->Get_FollowCam()->Start_ShakingCamera(0.08f, 0.3f);
		static_cast<CHit_Ground*>(CState_Manager::Get_Instance()->Get_State(STATE_HIT_GROUND_ENEMY))->On_HitGround(
			CHit_Ground::SPINBLOW, 13.f, 10.f, 10.f);
	}

}

HRESULT CSharkBombEffect::Start()
{
	CGameObject::Start();

	GET_COMPONENT(CPhysics)->Set_NaviOn();

	_float4 vDir = m_vTargetPosition - m_pTransform->Get_MyWorld(WORLD_POS);
	GET_COMPONENT(CPhysics)->Set_Dir(vDir);
	GET_COMPONENT(CPhysics)->Set_MaxSpeed(m_fMoveSpeed);
	GET_COMPONENT(CPhysics)->Set_Speed(m_fMoveSpeed);
	m_pTransform->Set_Look(vDir);


	m_EffectsList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SharkBomb", this, m_pTransform->Get_MyWorld(WORLD_POS));

	//list<CGameObject*>	StartList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SharkBoom", this, m_pTransform->Get_MyWorld(WORLD_POS));
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SharkParticle", this, m_pTransform->Get_MyWorld(WORLD_POS));

	CallBack_CollisionEnter += bind(&CSharkBombEffect::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3);


	return S_OK;
}

void CSharkBombEffect::My_Tick()
{
	GET_COMPONENT(CPhysics)->Set_MaxSpeed(m_fMoveSpeed);
	GET_COMPONENT(CPhysics)->Set_Speed(m_fMoveSpeed);

	m_fPCAcc += fDT;

	if (m_fPCAcc > m_fParticleCreateTime)
	{
		m_fPCAcc = 0.f;
		list<CGameObject*> ParticlesList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SharkParticle", this, m_pTransform->Get_MyWorld(WORLD_POS));
	}
}

void CSharkBombEffect::My_LateTick()
{
	//벽에 박히면 사라지게
	if (GET_COMPONENT(CNavigation)->Is_OnWall() ||
		GET_COMPONENT(CNavigation)->Is_Blocked())
	{
		DISABLE_GAMEOBJECT(this);
		return;
	}


	//__super::My_LateTick();
}

void CSharkBombEffect::OnEnable()
{
	_float4 vDir = m_vTargetPosition - m_pTransform->Get_MyWorld(WORLD_POS);
	GET_COMPONENT(CPhysics)->Set_Dir(vDir);
	m_pTransform->Set_Look(vDir);
	GET_COMPONENT(CNavigation)->Reset_OnWall();

	m_EffectsList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SharkBomb", this, m_pTransform->Get_MyWorld(WORLD_POS));

	//list<CGameObject*>	StartList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SharkBoom", this, m_pTransform->Get_MyWorld(WORLD_POS));
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SharkParticle", this, m_pTransform->Get_MyWorld(WORLD_POS));


	__super::OnEnable();
}

void CSharkBombEffect::OnDisable()
{
	__super::OnDisable();
	/*DISABLE_GAMEOBJECT(m_pTrailEffect);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pTrailEffect, CMesh))->Set_TrailOff();*/

	for (auto& elem : m_EffectsList)
		DISABLE_GAMEOBJECT(elem);

	CFunctor::Play_Sound(L"Eff_FireBomb", CHANNEL_EFFECTS, m_pTransform->Get_World(WORLD_POS));

	m_EffectsList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SharkBoom", this, m_pTransform->Get_MyWorld(WORLD_POS));

	if (GET_COMPONENT(CCollider_Sphere)->Get_ColIndex() == COL_ENEMYTHROW)
	{
		static_cast<CEffect*>(m_EffectsList.back())->Set_ColliderOn(3.f, COL_ENEMYSKILL);
		static_cast<CHit_Ground_Player*>(CState_Manager::Get_Instance()->Get_State(STATE_HIT_GROUND_PLAYER))->On_HitGround(
			CHit_Ground_Player::SPINBLOW, 13.f, 10.f, 10.f);
	}
	else
	{
		static_cast<CEffect*>(m_EffectsList.back())->Set_ColliderOn(3.f, COL_PLAYERSKILL);
		static_cast<CHit_Ground*>(CState_Manager::Get_Instance()->Get_State(STATE_HIT_GROUND_ENEMY))->On_HitGround(
			CHit_Ground::SPINBLOW, 13.f, 10.f, 10.f);
	}

	CUser::Get_Instance()->Get_FollowCam()->Start_ShakingCamera(0.08f, 0.3f);


}

HRESULT CSharkBombEffect::Initialize_Prototype()
{
	m_eDisableType = NONE;
	m_hcMyCode = HASHCODE(CSharkBombEffect);

	m_fMoveSpeed = 15.f;

	Add_Component(CComponent_Factory::Clone_Component<CPhysics>(this));
	GET_COMPONENT(CPhysics)->Set_Dir(_float4(0.f, 0.f, 0.f, 0.f));
	GET_COMPONENT(CPhysics)->Set_MaxSpeed(m_fMoveSpeed);
	GET_COMPONENT(CPhysics)->Set_Speed(m_fMoveSpeed);

	CCollider_Sphere* pCollider = CComponent_Factory::Clone_Component<CCollider_Sphere>(this);
	pCollider->Get_ColInfo().fRadius = 3.f;
	pCollider->Set_ColIndex(COL_BODY);
	Add_Component(pCollider);


	CNavigation* pNaviCom = CNavigation::Create(CP_NAVIGATION, nullptr, GET_COMPONENT(CPhysics));

	if (!pNaviCom)
		return E_FAIL;

	Add_Component(pNaviCom);

	return S_OK;
}

HRESULT CSharkBombEffect::Initialize()
{
	GET_COMPONENT(CCollider_Sphere)->Initialize();
	GET_COMPONENT(CCollider_Sphere)->Set_ColIndex(COL_BODY);

	/*m_pTrailEffect = CTrailEffect::Create(CP_RIGHTBEFORE_RENDERER, 10, _float4(0.03f, 0.f, 0.f, 1.f), _float4(-0.03f, 0.f, 0.f, 1.f)
		, nullptr, m_pTransform, _float4(0.f, 0.5f, 1.0f, 0.5f), _float4(1.f, 1.f, 1.f, 3.f),
		L"../bin/resources/textures/effects/gradientMap/T_EFF_Blur_04_M.dds",
		L"../bin/resources/textures/effects/gradationcolor/T_EFF_GMS_Light_01_BC.png"
	);
	m_pTrailEffect->Initialize();
	CREATE_GAMEOBJECT(m_pTrailEffect, GROUP_EFFECT);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pTrailEffect, CMesh))->Set_TrailOn();
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pTrailEffect, CMesh))->Set_NoCurve();


	if (!m_pTrailEffect)
		return E_FAIL;*/

	return S_OK;
}
