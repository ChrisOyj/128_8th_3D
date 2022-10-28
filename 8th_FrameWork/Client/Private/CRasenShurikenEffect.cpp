#include "stdafx.h"
#include "CRasenShurikenEffect.h"

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
#include "CRasenShurikenBomb.h"
#include "CUnit_Player.h"
#include "Functor.h"

#include "CCell.h"

CRasenShurikenEffect::CRasenShurikenEffect()
{
}

CRasenShurikenEffect::~CRasenShurikenEffect()
{
}

CRasenShurikenEffect* CRasenShurikenEffect::Create()
{
	CRasenShurikenEffect* pInstance = new CRasenShurikenEffect;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CRasenShurikenEffect");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CRasenShurikenEffect::On_Shoot(CGameObject* pFollowTarget, _float4 vTargetPos)
{
	CEffects_Factory::Get_Instance()->Create_ScreenEffects(this);

	//이전 주인의 오른쪽 대각선 방향
	_float4 vDir = m_pFollowTarget->Get_Transform()->Get_World(WORLD_LOOK) * 3.f + m_pFollowTarget->Get_Transform()->Get_World(WORLD_RIGHT);

	m_fFadeTimeAcc = 0.f;
	m_pFollowTarget = pFollowTarget;
	m_bShoot = true;
	m_vTargetPosition = vTargetPos;
	m_vTargetPosition.y += 1.f;
	m_pTransform->Set_Look(_float4(0.f, 0.f, 1.f, 0.f));
	m_fParticleCreateTime = 0.1f;

	GET_COMPONENT(CPhysics)->Set_SpeedasMax();


	if (m_pFollowTarget)
		m_vTargetPosition = m_pFollowTarget->Get_Transform()->Get_World(WORLD_POS);

	_float4 vTargetDir = m_vTargetPosition - m_pTransform->Get_World(WORLD_POS);
	vTargetDir.Normalize();
	vTargetDir += vDir.Normalize();

	vTargetDir.Normalize();

	GET_COMPONENT(CPhysics)->Set_Dir(vTargetDir);


	m_pTransform->Set_Look(vTargetDir);

	//Stick_RefBone();

	ENABLE_COMPONENT(GET_COMPONENT(CCollider_Sphere));
}

void CRasenShurikenEffect::OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos)
{
	if (dynamic_cast<CUnit_Player*>(pOtherObj) != nullptr)
		return;


	DISABLE_GAMEOBJECT(this);
}

HRESULT CRasenShurikenEffect::Start()
{
	CGameObject::Start();


	GET_COMPONENT(CPhysics)->Set_NaviOn();

	_float4 vDir = m_vTargetPosition - m_pTransform->Get_MyWorld(WORLD_POS);
	GET_COMPONENT(CPhysics)->Set_Dir(vDir);
	GET_COMPONENT(CPhysics)->Set_MaxSpeed(m_fMoveSpeed);
	GET_COMPONENT(CPhysics)->Set_Speed(0.f);
	m_pTransform->Set_Look(vDir);
	DISABLE_COMPONENT(GET_COMPONENT(CCollider_Sphere));


	m_EffectsList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"RasenShuriken_Ball", this, m_pTransform->Get_MyWorld(WORLD_POS));

	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"RasenShuriken_Start", this, m_pTransform->Get_World(WORLD_POS));
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"RasenShuriken_Particle", this, m_pTransform->Get_MyWorld(WORLD_POS));


	CallBack_CollisionEnter += bind(&CRasenShurikenEffect::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3);

	LIGHTDESC			LightDesc;

	LightDesc.eType = tagLightDesc::TYPE_POINT;

	LightDesc.fRange = 6.f;

	LightDesc.vDiffuse = _float4(0.f, 0.3f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.pOwner = this;

	GAMEINSTANCE->Add_Light(LightDesc);

	return S_OK;
}

void CRasenShurikenEffect::My_Tick()
{
	//뼈에 붙어있다가
	if (!m_bShoot)
	{
		Stick_RefBone();
	}
	else
	{
		GET_COMPONENT(CPhysics)->Get_Physics().bAir = true;


		//던지면 날라가기
		if (m_pFollowTarget)
		{
			m_vTargetPosition = m_pFollowTarget->Get_Transform()->Get_World(WORLD_POS);
			m_vTargetPosition.y += 0.8f;
		}

		CPhysics* pPhyscis = GET_COMPONENT(CPhysics);
		_float4 vMyPos = m_pTransform->Get_World(WORLD_POS);
		_float4 vTargetDir = m_vTargetPosition - vMyPos;
		_float4 vCurDir = pPhyscis->Get_Physics().vDir;


		pPhyscis->Get_Physics().vDir += (vTargetDir.Normalize() - vCurDir) * 1.5f * fDT;


		On_Target();
	}


	GET_COMPONENT(CPhysics)->Get_Physics().bAir = true;

	m_fPCAcc += fDT;

	if (m_fPCAcc > m_fParticleCreateTime)
	{
		m_fPCAcc = 0.f;
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"RasenShuriken_Particle", this, m_pTransform->Get_MyWorld(WORLD_POS));
	}
}

void CRasenShurikenEffect::My_LateTick()
{
	//벽에 박히면 사라지게
	if (GET_COMPONENT(CNavigation)->Is_OnWall() ||
		GET_COMPONENT(CNavigation)->Is_Blocked()
		|| !GET_COMPONENT(CPhysics)->Get_Physics().bAir)
	{
		DISABLE_GAMEOBJECT(this);
		return;
	}

	Update_Fade();



	//__super::My_LateTick();
}

void CRasenShurikenEffect::OnEnable()
{
	GET_COMPONENT(CPhysics)->Set_Speed(0.f);

	_float4 vDir = m_vTargetPosition - m_pTransform->Get_MyWorld(WORLD_POS);
	GET_COMPONENT(CPhysics)->Set_Dir(vDir);
	m_pTransform->Set_Look(vDir);
	GET_COMPONENT(CNavigation)->Reset_OnWall();
	m_bShoot = false;
	m_EffectsList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"RasenShuriken_Ball", this, m_pTransform->Get_MyWorld(WORLD_POS));
	m_fPCAcc = 0.f;
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"RasenShuriken_Start", this, m_pTransform->Get_World(WORLD_POS));
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"RasenShuriken_Particle", this, m_pTransform->Get_MyWorld(WORLD_POS));

	m_fParticleCreateTime = 0.3f;

	DISABLE_COMPONENT(GET_COMPONENT(CCollider_Sphere));

	static_cast<CHit_Ground*>(CState_Manager::Get_Instance()->Get_State(STATE_HIT_GROUND_ENEMY))->On_HitGround(
		CHit_Ground::HEAD, 1.5f, 1.5f, 0.1f);

	__super::OnEnable();

	LIGHTDESC			LightDesc;

	LightDesc.eType = tagLightDesc::TYPE_POINT;

	LightDesc.fRange = 6.f;

	LightDesc.vDiffuse = _float4(0.f, 0.3f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.pOwner = this;

	GAMEINSTANCE->Add_Light(LightDesc);
}

void CRasenShurikenEffect::OnDisable()
{
	__super::OnDisable();

	for (auto& elem : m_EffectsList)
		DISABLE_GAMEOBJECT(elem);
	CFunctor::Play_Sound(L"Eff_RasenShuriken_Boom", CHANNEL_EFFECTS, m_pTransform->Get_World(WORLD_POS));

	CGameObject* pEffect = CEffects_Factory::Get_Instance()->Create_Effects(HASHCODE(CRasenShurikenBomb), this, m_pTransform->Get_World(WORLD_POS));

	CCell* pCell = nullptr;

	if (GET_COMPONENT(CNavigation)->Get_TargetY() + 4.f > m_pTransform->Get_World(WORLD_POS).y)
	{
		pCell = GET_COMPONENT(CNavigation)->Get_CurCell();
	}

	static_cast<CRasenShurikenBomb*>(pEffect)->Set_Cell(pCell);

	//DISABLE_GAMEOBJECT(static_cast<CEffect*>(m_EffectsList.back()));

	//m_EffectsList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"FireBallExplosion", this, m_pTransform->Get_MyWorld(WORLD_POS));



	//static_cast<CEffect*>(m_EffectsList.front())->Set_ColliderOn(2.5f, COL_PLAYERSKILL);

	//CUser::Get_Instance()->Get_FollowCam()->Start_ShakingCamera(0.08f, 0.3f);


}

HRESULT CRasenShurikenEffect::Initialize_Prototype()
{
	m_eDisableType = FADE;
	m_fFadeOutStartTime = 3.f;
	m_hcMyCode = HASHCODE(CRasenShurikenEffect);

	m_fMoveSpeed = 50.f;

	Add_Component(CComponent_Factory::Clone_Component<CPhysics>(this));
	GET_COMPONENT(CPhysics)->Set_Dir(_float4(0.f, 0.f, 0.f, 0.f));
	GET_COMPONENT(CPhysics)->Set_MaxSpeed(m_fMoveSpeed);
	GET_COMPONENT(CPhysics)->Set_Speed(m_fMoveSpeed);
	GET_COMPONENT(CPhysics)->Get_Physics().fGravity = 0.f;
	GET_COMPONENT(CPhysics)->Get_Physics().bAir = true;

	CCollider_Sphere* pCollider = CComponent_Factory::Clone_Component<CCollider_Sphere>(this);
	pCollider->Get_ColInfo().fRadius = 2.f;
	pCollider->Set_ColIndex(COL_BODY);
	Add_Component(pCollider);


	CNavigation* pNaviCom = CNavigation::Create(CP_NAVIGATION, nullptr, GET_COMPONENT(CPhysics));

	if (!pNaviCom)
		return E_FAIL;

	Add_Component(pNaviCom);

	return S_OK;
}

HRESULT CRasenShurikenEffect::Initialize()
{
	GET_COMPONENT(CCollider_Sphere)->Initialize();
	GET_COMPONENT(CCollider_Sphere)->Set_ColIndex(COL_BODY);

	return S_OK;
}
