#include "stdafx.h"
#include "CEveElephant.h"

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

CEveElephant::CEveElephant()
{
}

CEveElephant::~CEveElephant()
{
}

CEveElephant* CEveElephant::Create()
{
	CEveElephant* pInstance = new CEveElephant;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CEveElephant");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CEveElephant::OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos)
{
	DISABLE_GAMEOBJECT(this);
}

HRESULT CEveElephant::Start()
{
	CGameObject::Start();

	GET_COMPONENT(CPhysics)->Set_NaviOn();

	_float4 vDir = m_vTargetPosition - m_pTransform->Get_MyWorld(WORLD_POS);
	GET_COMPONENT(CPhysics)->Set_Dir(vDir);
	GET_COMPONENT(CPhysics)->Set_MaxSpeed(m_fMoveSpeed);
	GET_COMPONENT(CPhysics)->Set_Speed(m_fMoveSpeed);
	m_pTransform->Set_Look(vDir);


	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"EightGatesAttack", this, m_pTransform->Get_MyWorld(WORLD_POS));
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"EveElephant"), this, m_pTransform->Get_MyWorld(WORLD_POS));
	CallBack_CollisionEnter += bind(&CEveElephant::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3);


		

	return S_OK;
}

void CEveElephant::My_Tick()
{
	GET_COMPONENT(CPhysics)->Get_Physics().bAir = true;
	GET_COMPONENT(CPhysics)->Set_MaxSpeed(m_fMoveSpeed);
	GET_COMPONENT(CPhysics)->Set_Speed(m_fMoveSpeed);
	m_fDistortionAcc += fDT;

	if (m_fDistortionAcc > m_fDistortionCreateTime)
	{
		m_fDistortionAcc = 0.f;
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"EightGatesAttack", this, m_pTransform->Get_MyWorld(WORLD_POS));
	}
}

void CEveElephant::My_LateTick()
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

void CEveElephant::OnEnable()
{
	_float4 vDir = m_vTargetPosition - m_pTransform->Get_MyWorld(WORLD_POS);
	GET_COMPONENT(CPhysics)->Set_Dir(vDir);
	m_pTransform->Set_Look(vDir);
	GET_COMPONENT(CNavigation)->Reset_OnWall();

	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"EightGatesAttack", this, m_pTransform->Get_MyWorld(WORLD_POS));
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"EveElephant"), this, m_pTransform->Get_MyWorld(WORLD_POS));

	__super::OnEnable();
}

void CEveElephant::OnDisable()
{
	__super::OnDisable();
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"EightGatesHit", this, m_pTransform->Get_MyWorld(WORLD_POS));


}

HRESULT CEveElephant::Initialize_Prototype()
{
	m_eDisableType = NONE;
	m_hcMyCode = HASHCODE(CEveElephant);


	m_fMoveSpeed = 100.f;

	Add_Component(CComponent_Factory::Clone_Component<CPhysics>(this));
	GET_COMPONENT(CPhysics)->Set_Dir(_float4(0.f, 0.f, 0.f, 0.f));
	GET_COMPONENT(CPhysics)->Set_MaxSpeed(m_fMoveSpeed);
	GET_COMPONENT(CPhysics)->Set_Speed(m_fMoveSpeed);
	GET_COMPONENT(CPhysics)->Get_Physics().fGravity = 0.f;
	GET_COMPONENT(CPhysics)->Get_Physics().bAir = true;

	CCollider_Sphere* pCollider = CComponent_Factory::Clone_Component<CCollider_Sphere>(this);
	pCollider->Get_ColInfo().fRadius = 3.f;
	pCollider->Set_ColIndex(COL_PLAYERSKILL);
	Add_Component(pCollider);


	CNavigation* pNaviCom = CNavigation::Create(CP_NAVIGATION, nullptr, GET_COMPONENT(CPhysics));

	if (!pNaviCom)
		return E_FAIL;

	Add_Component(pNaviCom);

	return S_OK;
}

HRESULT CEveElephant::Initialize()
{
	GET_COMPONENT(CCollider_Sphere)->Initialize();
	GET_COMPONENT(CCollider_Sphere)->Set_ColIndex(COL_PLAYERSKILL);

	return S_OK;
}
