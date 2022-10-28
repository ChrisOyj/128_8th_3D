#include "stdafx.h"
#include "CWindmill.h"

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

#include "CEffects_Factory.h"
#include "Functor.h"
CWindmill::CWindmill()
{
}

CWindmill::~CWindmill()
{
}

CWindmill* CWindmill::Create()
{
	CWindmill* pInstance = new CWindmill;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CWindmill");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CWindmill::On_Shoot(CGameObject* pFollowTarget, _float4 vTargetPos)
{
	CEffects_Factory::Get_Instance()->Create_ScreenEffects(this);

	//이전 주인의 오른쪽 대각선 방향
	_float4 vDir = m_pFollowTarget->Get_Transform()->Get_World(WORLD_LOOK) * 3.f + m_pFollowTarget->Get_Transform()->Get_World(WORLD_RIGHT);
	GET_COMPONENT(CPhysics)->Set_Dir(vDir);

	m_fFadeTimeAcc = 0.f;
	m_pFollowTarget = pFollowTarget;
	m_bShoot = true;
	m_vTargetPosition = vTargetPos;
	m_vTargetPosition.y += 1.f;
	m_pTransform->Set_Look(_float4(0.f, 0.f, 1.f, 0.f));

	GET_COMPONENT(CPhysics)->Set_TurnSpeed(30.f);
	//Stick_RefBone();

	ENABLE_COMPONENT(GET_COMPONENT(CCollider_Sphere));

	m_pEffect = CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"WindMill_0"), this, m_pTransform->Get_World(WORLD_POS));

	CFunctor::Play_Sound(L"Eff_WindMill", CHANNEL_EFFECTS, m_pTransform->Get_World(WORLD_POS));
	m_fSoundAcc = 0.f;

}

void CWindmill::OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos)
{
	if (eColType == COL_ENEMYHITBOX)
		CFunctor::Play_Sound(L"Eff_Hit_Hard", CHANNEL_EFFECTS, m_pTransform->Get_World(WORLD_POS));


	DISABLE_GAMEOBJECT(this);
}

HRESULT CWindmill::Start()
{
	__super::Start();
	_float4 vDir = m_vTargetPosition - m_pTransform->Get_MyWorld(WORLD_POS);
	GET_COMPONENT(CPhysics)->Set_NaviOn();
	GET_COMPONENT(CPhysics)->Set_Dir(vDir);
	GET_COMPONENT(CPhysics)->Set_TurnSpeed(0.f);
	GET_COMPONENT(CPhysics)->Set_Speed(0.f);
	m_fTimeAcc = 0.f;
	DISABLE_COMPONENT(GET_COMPONENT(CCollider_Sphere));
	//m_fTurnSpeed = 0.f;
	m_bShoot = false;

	return S_OK;
}

void CWindmill::My_Tick()
{
	//뼈에 붙어있다가
	if (!m_bShoot)
	{
		Stick_RefBone();
	}
	else
	{
		



		//벽에 박히면 사라지게
		if (!m_bOnWall && GET_COMPONENT(CNavigation)->Is_OnWall())
		{
			CFunctor::Play_Sound(L"Eff_WireHit", CHANNEL_EFFECTS, m_pTransform->Get_World(WORLD_POS));

			m_bOnWall = true;
			GET_COMPONENT(CPhysics)->Set_TurnSpeed(0.f);
			GET_COMPONENT(CPhysics)->Set_Speed(0.f);
			m_eDisableType = FADE;
			m_fFadeOutStartTime = 2.f;
			m_fFadeTimeAcc = 0.f;
			DISABLE_COMPONENT(GET_COMPONENT(CCollider_Sphere));

			DISABLE_GAMEOBJECT(m_pEffect);
			m_pEffect = nullptr;
			return;
		}

		if (m_bOnWall)
		{
			Update_Fade();
			return;
		}

		m_fSoundAcc += fDT;
		if (m_fSoundAcc > m_fSoundTime)
		{
			CFunctor::Play_Sound(L"Eff_WindMill", CHANNEL_EFFECTS, m_pTransform->Get_World(WORLD_POS));
			m_fSoundAcc = 0.f;
		}


		GET_COMPONENT(CPhysics)->Get_Physics().bAir = true;

		m_fCreateAcc += fDT;
		if (m_fCreateAcc > m_fCreateTime)
		{
			m_fCreateAcc = 0.f;
			CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"WindSuriAfterEffect"), m_pTransform->Get_World(WORLD_POS));
		}
		//던지면 날라가기
		if (m_pFollowTarget)
		{
			m_vTargetPosition = m_pFollowTarget->Get_Transform()->Get_World(WORLD_POS);
			m_vTargetPosition.y += 1.f;
		}

		CPhysics* pPhyscis = GET_COMPONENT(CPhysics);
		_float4 vMyPos = m_pTransform->Get_World(WORLD_POS);
		_float4 vTargetDir = m_vTargetPosition - vMyPos;
		_float4 vCurDir = pPhyscis->Get_Physics().vDir;
		pPhyscis->Get_Physics().vDir += (vTargetDir.Normalize() - vCurDir) * 1.f * fDT;

		On_Target();
	}
}

void CWindmill::My_LateTick()
{
	Update_Fade();

}

void CWindmill::OnEnable()
{
	_float4 vDir = m_vTargetPosition - m_pTransform->Get_MyWorld(WORLD_POS);
	GET_COMPONENT(CPhysics)->Set_Dir(vDir);
	//m_pTransform->Set_Look(vDir);
	m_bShoot = false;
	m_fTimeAcc = 0.f;
	GET_COMPONENT(CPhysics)->Set_Speed(0.f);
	GET_COMPONENT(CPhysics)->Set_TurnSpeed(0.f);
	DISABLE_COMPONENT(GET_COMPONENT(CCollider_Sphere));


}

void CWindmill::OnDisable()
{
	__super::OnDisable();
	m_bOnWall = false;

	if (m_pEffect)
		DISABLE_GAMEOBJECT(m_pEffect);

	m_pEffect = nullptr;
}

HRESULT CWindmill::Initialize_Prototype()
{
	m_eShaderType = SHADER_VTXMODEL;
	m_eDisableType = FADE;
	m_fFadeOutStartTime = 3.f;
	m_wstrPath = L"../bin/resources/meshes/weapons/Throwing/SM_WEP_WindmillShuriken.fbx";
	m_matTrans = XMMatrixScaling(0.02f, 0.02f, 0.02f) * XMMatrixRotationX(XMConvertToRadians(90.f));
	m_hcMyCode = HASHCODE(CWindmill);
	m_iPassType = VTXMODEL_PASS_DEFAULT;
	m_fMoveSpeed = 30.f;
	m_bEffectFlag = 0;
	m_vEffectFlag = SH_LIGHT_DEFAULT;
	m_fTurnSpeed = 30.f;


	Add_Component(CComponent_Factory::Clone_Component<CPhysics>(this));
	GET_COMPONENT(CPhysics)->Set_Dir(_float4(0.f, 0.f, 0.f, 0.f));
	GET_COMPONENT(CPhysics)->Set_MaxSpeed(m_fMoveSpeed);
	GET_COMPONENT(CPhysics)->Set_TurnDir(_float4(0.f, -1.f, 0.f, 0.f));
	GET_COMPONENT(CPhysics)->Get_Physics().fGravity = 0.f;
	GET_COMPONENT(CPhysics)->Get_Physics().bAir = true;
	GET_COMPONENT(CPhysics)->Get_PhysicsDetail().fFrictionRatio = 0.f;

	CCollider_Sphere* pCollider = CComponent_Factory::Clone_Component<CCollider_Sphere>(this);
	pCollider->Get_ColInfo().fRadius = 1.f;
	pCollider->Set_ColIndex(COL_PLAYERTHROW);
	Add_Component(pCollider);


	CNavigation* pNaviCom = CNavigation::Create(CP_NAVIGATION, nullptr, GET_COMPONENT(CPhysics));

	if (!pNaviCom)
		return E_FAIL;

	Add_Component(pNaviCom);


	return __super::Initialize_Prototype();
}

HRESULT CWindmill::Initialize()
{
	GET_COMPONENT(CCollider_Sphere)->Initialize();
	GET_COMPONENT(CCollider_Sphere)->Set_ColIndex(COL_PLAYERTHROW);
	
	GET_COMPONENT(CModel)->Set_ShaderFlag(SH_LIGHT_DEFAULT);
	GET_COMPONENT(CRenderer)->Set_RenderGroup(RENDER_NONALPHA);

	static_cast<CModel_Renderer*>(GET_COMPONENT(CRenderer))->Set_ShadowType(CModel_Renderer::DEFAULTSHADOW);


	return S_OK;
}
