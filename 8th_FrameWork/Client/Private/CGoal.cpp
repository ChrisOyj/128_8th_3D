#include "stdafx.h"
#include "CGoal.h"

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

#include "CCamera_Follow.h"

#include "CUtility_Transform.h"
#include "CUser.h"
#include "CTargetObject.h"
#include "CUnit_Player.h"

#include "CState_Manager.h"
#include "CPose.h"
#include "CEffect.h"
#include "CEffects_Factory.h"

#include "Functor.h"

CGoal::CGoal()
{
}

CGoal::~CGoal()
{
}

CGoal* CGoal::Create()
{
	CGoal* pInstance = new CGoal;


	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CGoal");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CGoal::OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos)
{
	CFunctor::Play_Sound(L"Eff_EnterGoal", CHANNEL_UI, m_pTransform->Get_World(WORLD_POS));



	switch (m_iEventIndex)
	{
	case 0:
		//첫 충돌, 다이얼로그 출력, 플레이어 정지,
		GAMEINSTANCE->Play_Sound(L"Voice_Boss_GoFight", (CHANNEL_GROUP)CHANNEL_VOICE);

		CUser::Get_Instance()->On_Dialog(0);
		CState_Manager::Get_Instance()->Get_State(STATE_POSE)->Set_AnimType(ANIM_WIN_TYPE_3);

		{
			_float4 vPlayerPos = PLAYER->Get_Transform()->Get_World(WORLD_POS);
			_float4 vEnemyPos = m_pTransform->Get_World(WORLD_POS);
			_float4 vDir = (vEnemyPos - vPlayerPos);
			vDir.y = 0.f;
			vDir.Normalize();
			PLAYER->Get_Transform()->Set_LerpLook(vDir, 0.4f);
			PLAYER->Enter_State(STATE_POSE);
		}
		


		break;

	case 1:
		//두번째 이벤트, 다이얼로그, 적 활성화


		CUser::Get_Instance()->On_Dialog(2);
		GAMEINSTANCE->Play_Sound(L"Voice_Boss_GoFight", (CHANNEL_GROUP)CHANNEL_VOICE);

		CUser::Get_Instance()->On_Stage2Event(5);

		break;

	case 2:
		//연출
		CUser::Get_Instance()->On_FinalEvent();
		break;

	default:
		break;
	}

	DISABLE_GAMEOBJECT(this);
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"CJ_MaxCharge_1"), m_pTransform->Get_WorldMatrix());

	m_iEventIndex++;
}

void CGoal::Enable_UI()
{
	m_pTargetObject->On_TargetGoal(this);
	m_pGoalEffect1 = CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Goal_0"), m_pTransform->Get_WorldMatrix());
	m_pGoalEffect2 = CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Goal_1"), m_pTransform->Get_WorldMatrix());

	//GET_COMPONENT_FROM(m_pGoalEffect2, CModel)->No_Culling();

}

HRESULT CGoal::Start()
{
	__super::Start();

	m_pTargetObject = CTargetObject::Create(this);

	if (FAILED(m_pTargetObject->Initialize()))
		return E_FAIL;
	CREATE_GAMEOBJECT(m_pTargetObject, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pTargetObject);

	CallBack_CollisionEnter += bind(&CGoal::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3);

	return S_OK;
}

void CGoal::My_Tick()
{

}

void CGoal::OnEnable()
{
	__super::OnEnable();
	m_pTransform->Make_WorldMatrix();
	Enable_UI();
}

void CGoal::OnDisable()
{
	__super::OnDisable();
	DISABLE_GAMEOBJECT(m_pTargetObject);
	static_cast<CEffect*>(m_pGoalEffect1)->Set_FadeOut();
	static_cast<CEffect*>(m_pGoalEffect2)->Set_FadeOut();

}

HRESULT CGoal::Initialize_Prototype()
{
	CCollider_Sphere* pCollider = CComponent_Factory::Clone_Component<CCollider_Sphere>(this);
	pCollider->Get_ColInfo().fRadius = 6.f;
	pCollider->Set_ColIndex(COL_EVENT);
	Add_Component(pCollider);
	return S_OK;
}

HRESULT CGoal::Initialize()
{
	GET_COMPONENT(CCollider_Sphere)->Initialize();
	GET_COMPONENT(CCollider_Sphere)->Set_ColIndex(COL_EVENT);


	return S_OK;
}