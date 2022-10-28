#include "stdafx.h"
#include "CBossKunai.h"

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

#include "CNavigation.h"
#include "CEffects_Factory.h"

#include "CHit_Ground_Player.h"
#include "CHit_Ground.h"
#include "CState_Manager.h"

#include "CUser.h"
#include "CScript_FollowCam.h"

#include "Functor.h"
CBossKunai::CBossKunai()
{
}

CBossKunai::~CBossKunai()
{
}

CBossKunai* CBossKunai::Create()
{
	CBossKunai* pInstance = new CBossKunai;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CBossKunai");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CBossKunai::Start()
{
	__super::Start();
	if (m_pFollowTarget)
	{
		m_vTargetPosition = m_pFollowTarget->Get_Transform()->Get_World(WORLD_POS);
		m_vTargetPosition.y += 0.6f;
	}
	
	_float4 vDir = m_vTargetPosition - m_pTransform->Get_MyWorld(WORLD_POS);
	GET_COMPONENT(CPhysics)->Set_Dir(vDir);
	GET_COMPONENT(CPhysics)->Set_SpeedasMax();
	GET_COMPONENT(CPhysics)->Set_NaviOn();
	GET_COMPONENT(CPhysics)->Get_Physics().fGravity = 0.f;
	m_pTransform->Set_Look(vDir);
	m_vOriginLook = vDir;


	return S_OK;
}

void CBossKunai::My_Tick()
{
	if (m_bOnTarget || m_bOnWall)
		return;

	if (m_pFollowTarget)
	{
		m_vTargetPosition = m_pFollowTarget->Get_Transform()->Get_World(WORLD_POS);
		m_vTargetPosition.y += 0.6f;
	}

	CPhysics* pPhyscis = GET_COMPONENT(CPhysics);
	_float4 vMyPos = m_pTransform->Get_World(WORLD_POS);
	_float4 vTargetDir = m_vTargetPosition - vMyPos;
	_float4 vCurDir = pPhyscis->Get_Physics().vDir;

	pPhyscis->Get_Physics().vDir += (vTargetDir.Normalize() - vCurDir) * 2.f * fDT;
	//pPhyscis->Get_Physics().vDir = vTargetDir;

	pPhyscis->Get_Physics().vDir.Normalize();
	pPhyscis->Set_Accel(50.f);
	pPhyscis->Get_Physics().bAir = true;
	m_pTransform->Set_Look(pPhyscis->Get_Physics().vDir);


	if (m_vOriginLook.Dot(pPhyscis->Get_Physics().vDir) < 0.4f)
	{
		DISABLE_GAMEOBJECT(this);
	}




}

void CBossKunai::My_LateTick()
{
	Update_Fade();
	//타겟과 닿아있는 상태
	if (m_bOnWall)
		return;

	if (m_bOnTarget)
	{
		_float4 vLook = m_vOnLook.MultiplyNormal(m_pFollowTarget->Get_Transform()->Get_WorldMatrix());
		vLook.Normalize();

		_float4 vPos = m_pFollowTarget->Get_Transform()->Get_World(WORLD_POS);
		_float4 vOffsetPos = m_vOffsetPos.MultiplyCoord(m_pFollowTarget->Get_Transform()->Get_WorldMatrix(MARTIX_NOTRANS));
		vPos += vOffsetPos;

		m_pTransform->Set_Look(vLook);
		m_pTransform->Set_World(WORLD_POS, vPos);

		m_pTransform->Make_WorldMatrix();
	}
	else
	{
		//타겟과 닿았는지 따져보자
		if (m_pFollowTarget)
		{
			m_vTargetPosition = m_pFollowTarget->Get_Transform()->Get_World(WORLD_POS);

			m_vTargetPosition.y += 0.6f;
		}
		


		_float4 vMyPos = m_pTransform->Get_World(WORLD_POS);

		_float fOriginY = vMyPos.y;
		vMyPos.y = m_vTargetPosition.y;

		if ((vMyPos - m_vTargetPosition).Length() < 0.3f)
		{
			if (!m_pFollowTarget)
			{
				DISABLE_GAMEOBJECT(this);
				return;
			}



			if ((fOriginY - m_vTargetPosition.y) < 0.5f)
			{
				//내위치에 상대 역행렬 곱하면 오프셋이 나옴.
				_float4x4 matWorldInv = m_pFollowTarget->Get_Transform()->Get_WorldMatrix().Inverse();
				m_vOffsetPos = vMyPos.MultiplyCoord(matWorldInv);
				m_vOnLook = m_pTransform->Get_World(WORLD_LOOK).MultiplyNormal(matWorldInv);
				m_bOnTarget = true;
				m_fFadeOutStartTime = 0.f;
				GET_COMPONENT(CPhysics)->Set_Speed(0.f);
				DISABLE_COMPONENT(GET_COMPONENT(CNavigation));
				CFunctor::Play_Sound(L"Eff_WireHit", CHANNEL_EFFECTS, m_pTransform->Get_World(WORLD_POS));

				return;
			}


			
		}

		if (GET_COMPONENT(CNavigation)->Is_Blocked() ||
			GET_COMPONENT(CNavigation)->Is_OnWall())
		{
			m_fFadeOutStartTime = 0.f;
			GET_COMPONENT(CPhysics)->Set_Speed(0.f);
			DISABLE_COMPONENT(GET_COMPONENT(CNavigation));
			m_bOnWall = true;
			CFunctor::Play_Sound(L"Eff_WireHit", CHANNEL_EFFECTS, m_pTransform->Get_World(WORLD_POS));

		}
	}

	


	m_fAlpha = 1.f;
}

void CBossKunai::OnEnable()
{
	if (m_pFollowTarget)
	{
		m_vTargetPosition = m_pFollowTarget->Get_Transform()->Get_World(WORLD_POS);
		m_vTargetPosition.y += 0.6f;
	}

	_float4 vDir = m_vTargetPosition - m_pTransform->Get_MyWorld(WORLD_POS);
	GET_COMPONENT(CPhysics)->Set_Dir(vDir);
	GET_COMPONENT(CPhysics)->Get_Physics().fGravity = 0.f;
	m_vOriginLook = vDir;

	GET_COMPONENT(CPhysics)->Set_SpeedasMax();
	GET_COMPONENT(CNavigation)->Reset_OnWall();
	m_pTransform->Set_Look(vDir);

	ENABLE_GAMEOBJECT(m_pTrailEffect);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pTrailEffect, CMesh))->Set_TrailOn();

}

void CBossKunai::OnDisable()
{

	CFunctor::Play_Sound(L"Eff_FireBomb", CHANNEL_EFFECTS, m_pTransform->Get_World(WORLD_POS));

	__super::OnDisable();
	m_pFollowTarget = nullptr;
	m_fFadeOutStartTime = 999.f;
	m_bOnTarget = false;
	m_bOnWall = false;
	DISABLE_GAMEOBJECT(m_pTrailEffect);
	static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_pTrailEffect, CMesh))->Set_TrailOff();

	list<CGameObject*> EffectsList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"FireBallExplosion", this, m_pTransform->Get_MyWorld(WORLD_POS));
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"FireBallStart", this, m_pTransform->Get_MyWorld(WORLD_POS));

	static_cast<CEffect*>(EffectsList.front())->Set_ColliderOn(2.5f, m_eColType);

	if (m_eColType == COL_ENEMYSKILL)
	{
		static_cast<CHit_Ground_Player*>(CState_Manager::Get_Instance()->Get_State(STATE_HIT_GROUND_PLAYER))->On_HitGround(
			CHit_Ground_Player::SPINBLOW, 13.f, 10.f, 5.f);
	}
	else
	{

		static_cast<CHit_Ground*>(CState_Manager::Get_Instance()->Get_State(STATE_HIT_GROUND_ENEMY))->On_HitGround(
			CHit_Ground::SPINBLOW, 13.f, 10.f, 5.f);
	}

	CUser::Get_Instance()->Get_FollowCam()->Start_ShakingCamera(0.08f, 0.3f);

}

HRESULT CBossKunai::Initialize_Prototype()
{
	m_eDisableType = FADE;
	m_fFadeOutStartTime = 999.f;
	m_fFadeOutTime = 3.f;
	m_wstrPath = L"../bin/resources/meshes/weapons/SM_WEP_Kunai_basic.fbx";
	m_matTrans = XMMatrixScaling(0.02f, 0.02f, 0.02f) * XMMatrixRotationY(XMConvertToRadians(-90.0f));
	m_hcMyCode = HASHCODE(CBossKunai);
	m_iPassType = VTXEFFECT_PASS_DEFAULT;
	m_fMoveSpeed = 40.f;
	m_bEffectFlag = EFFECT_TARGETPOS;

	m_vEffectFlag = SH_EFFECT_NOBLOOM;
	m_vGlowFlag = _float4(1.f, 0.f, 0.f, 0.7f);

	Add_Component(CComponent_Factory::Clone_Component<CPhysics>(this));
	GET_COMPONENT(CPhysics)->Set_Dir(_float4(0.f, 0.f, 0.f, 0.f));
	GET_COMPONENT(CPhysics)->Set_MaxSpeed(m_fMoveSpeed);


	CNavigation* pNaviCom = CNavigation::Create(CP_NAVIGATION, nullptr, GET_COMPONENT(CPhysics));

	if (!pNaviCom)
		return E_FAIL;

	Add_Component(pNaviCom);

	return __super::Initialize_Prototype();
}

HRESULT CBossKunai::Initialize()
{
	m_pTrailEffect = CTrailEffect::Create(CP_RIGHTBEFORE_RENDERER, 20, _float4(0.03f, 0.f, 0.f, 1.f), _float4(-0.03f, 0.f, 0.f, 1.f)
		, nullptr, m_pTransform, _float4(1.f, 0.f, 0.f, 0.5f), _float4(1.f, 1.f, 1.f, 3.f),
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
