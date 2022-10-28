#include "stdafx.h"
#include "CFireBallEffect.h"

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

CFireBallEffect::CFireBallEffect()
{
}

CFireBallEffect::~CFireBallEffect()
{
}

CFireBallEffect* CFireBallEffect::Create()
{
	CFireBallEffect* pInstance = new CFireBallEffect;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CFireBallEffect");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CFireBallEffect::OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos)
{
	DISABLE_GAMEOBJECT(this);
}

HRESULT CFireBallEffect::Start()
{
	CGameObject::Start();

	LIGHTDESC			LightDesc;

	LightDesc.eType = tagLightDesc::TYPE_POINT;

	LightDesc.fRange = 6.f;

	LightDesc.vDiffuse = _float4(1.f, 0.2f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.pOwner = this;

	GAMEINSTANCE->Add_Light(LightDesc);

	GET_COMPONENT(CPhysics)->Set_NaviOn();

	_float4 vDir = m_vTargetPosition - m_pTransform->Get_MyWorld(WORLD_POS);
	GET_COMPONENT(CPhysics)->Set_Dir(vDir);
	GET_COMPONENT(CPhysics)->Set_MaxSpeed(m_fMoveSpeed);
	GET_COMPONENT(CPhysics)->Set_Speed(m_fMoveSpeed);
	m_pTransform->Set_Look(vDir);


	m_EffectsList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"FireBalls", this, m_pTransform->Get_MyWorld(WORLD_POS));
	GET_COMPONENT_FROM(m_EffectsList.front(), CModel)->Get_MeshContainers().front().second->Set_Color(_float4(1.5f, 1.5f, 1.5f, 1.f));

	list<CGameObject*>	StartList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"FireBallStart", this, m_pTransform->Get_MyWorld(WORLD_POS));



	LightDesc.eType = tagLightDesc::TYPE_POINT;

	LightDesc.fRange = 6.f;

	LightDesc.vDiffuse = _float4(0.8f, 0.3f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(0.3f, 0.3f, 0.3f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.pOwner = StartList.front();

	GAMEINSTANCE->Add_Light(LightDesc);


	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"FireBallParticle", this, m_pTransform->Get_MyWorld(WORLD_POS));

	static_cast<CEffect*>(m_EffectsList.back())->Set_EffectFlag(EFFECT_FOLLOWTARGET);

	CallBack_CollisionEnter += bind(&CFireBallEffect::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3);


	return S_OK;
}

void CFireBallEffect::My_Tick()
{
	GET_COMPONENT(CPhysics)->Get_Physics().bAir = true;
	GET_COMPONENT(CPhysics)->Set_MaxSpeed(m_fMoveSpeed);
	GET_COMPONENT(CPhysics)->Set_Speed(m_fMoveSpeed);

	m_fPCAcc += fDT;

	if (m_fPCAcc > m_fParticleCreateTime)
	{
		m_fPCAcc = 0.f;
		list<CGameObject*> ParticlesList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"FireBallParticle", this, m_pTransform->Get_MyWorld(WORLD_POS));
	}

	m_fDistortionAcc += fDT;

	if (m_fDistortionAcc > m_fDistortionCreateTime)
	{
		m_fDistortionAcc = 0.f;
		CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"FireBall_Distortion_0"), this, m_pTransform->Get_MyWorld(WORLD_POS));
	}
}

void CFireBallEffect::My_LateTick()
{
	//벽에 박히면 사라지게
	if (GET_COMPONENT(CNavigation)->Is_OnWall() ||
		GET_COMPONENT(CNavigation)->Is_Blocked() 
		|| !GET_COMPONENT(CPhysics)->Get_Physics().bAir)
	{
		DISABLE_GAMEOBJECT(this);
		return;
	}


	//__super::My_LateTick();
}

void CFireBallEffect::OnEnable()
{
	_float4 vDir = m_vTargetPosition - m_pTransform->Get_MyWorld(WORLD_POS);
	GET_COMPONENT(CPhysics)->Set_Dir(vDir);
	m_pTransform->Set_Look(vDir);
	GET_COMPONENT(CNavigation)->Reset_OnWall();

	m_EffectsList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"FireBalls", this, m_pTransform->Get_MyWorld(WORLD_POS));
	list<CGameObject*>	StartList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"FireBallStart", this, m_pTransform->Get_MyWorld(WORLD_POS));


	LIGHTDESC			LightDesc;

	LightDesc.eType = tagLightDesc::TYPE_POINT;

	LightDesc.fRange = 6.f;

	LightDesc.vDiffuse = _float4(0.8f, 0.3f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(0.3f, 0.3f, 0.3f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.pOwner = StartList.front();

	GAMEINSTANCE->Add_Light(LightDesc);
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"FireBallParticle", this, m_pTransform->Get_MyWorld(WORLD_POS));



	LightDesc.eType = tagLightDesc::TYPE_POINT;

	LightDesc.fRange = 6.f;

	LightDesc.vDiffuse = _float4(1.f, 0.2f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.pOwner = this;

	GAMEINSTANCE->Add_Light(LightDesc);





	__super::OnEnable();
}

void CFireBallEffect::OnDisable()
{
	__super::OnDisable();
	/*DISABLE_GAMEOBJECT(m_pTrailEffect);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pTrailEffect, CMesh))->Set_TrailOff();*/

	for (auto& elem : m_EffectsList)
		static_cast<CEffect*>(elem)->Set_FadeOut();

	DISABLE_GAMEOBJECT(static_cast<CEffect*>(m_EffectsList.back()));

	m_EffectsList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"FireBallExplosion", this, m_pTransform->Get_MyWorld(WORLD_POS));
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"FireBallStart", this, m_pTransform->Get_MyWorld(WORLD_POS));
	
	if (GET_COMPONENT(CCollider_Sphere)->Get_ColIndex() == COL_ENEMYTHROW)
	{
		static_cast<CEffect*>(m_EffectsList.front())->Set_ColliderOn(2.5f, COL_ENEMYSKILL);
		static_cast<CHit_Ground_Player*>(CState_Manager::Get_Instance()->Get_State(STATE_HIT_GROUND_PLAYER))->On_HitGround(
			CHit_Ground_Player::SPINBLOW, 13.f, 10.f, 20.f);
	}
	else
	{
		static_cast<CEffect*>(m_EffectsList.front())->Set_ColliderOn(2.5f, COL_PLAYERSKILL);
		static_cast<CHit_Ground*>(CState_Manager::Get_Instance()->Get_State(STATE_HIT_GROUND_ENEMY))->On_HitGround(
			CHit_Ground::SPINBLOW, 13.f, 10.f, 20.f);
	}

	LIGHTDESC			LightDesc;

	LightDesc.eType = tagLightDesc::TYPE_POINT;

	LightDesc.fRange = 10.f;

	LightDesc.vDiffuse = _float4(1.0f, 0.2f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(0.3f, 0.3f, 0.3f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.pOwner = m_EffectsList.front();

	GAMEINSTANCE->Add_Light(LightDesc);
	

	CUser::Get_Instance()->Get_FollowCam()->Start_ShakingCamera(0.08f, 0.3f);

	
	CFunctor::Play_Sound(L"Eff_FireBallBomb", CHANNEL_EFFECTS, m_pTransform->Get_World(WORLD_POS));

}

HRESULT CFireBallEffect::Initialize_Prototype()
{
	m_eDisableType = NONE;
	m_hcMyCode = HASHCODE(CFireBallEffect);


	m_fMoveSpeed = 50.f;

	Add_Component(CComponent_Factory::Clone_Component<CPhysics>(this));
	GET_COMPONENT(CPhysics)->Set_Dir(_float4(0.f, 0.f, 0.f, 0.f));
	GET_COMPONENT(CPhysics)->Set_MaxSpeed(m_fMoveSpeed);
	GET_COMPONENT(CPhysics)->Set_Speed(m_fMoveSpeed);
	GET_COMPONENT(CPhysics)->Get_Physics().fGravity = 0.f;
	GET_COMPONENT(CPhysics)->Get_Physics().bAir = true;

	CCollider_Sphere* pCollider = CComponent_Factory::Clone_Component<CCollider_Sphere>(this);
	pCollider->Get_ColInfo().fRadius = 0.25f;
	pCollider->Set_ColIndex(COL_BODY);
	Add_Component(pCollider);


	CNavigation* pNaviCom = CNavigation::Create(CP_NAVIGATION, nullptr, GET_COMPONENT(CPhysics));

	if (!pNaviCom)
		return E_FAIL;

	Add_Component(pNaviCom);

	return S_OK;
}

HRESULT CFireBallEffect::Initialize()
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
