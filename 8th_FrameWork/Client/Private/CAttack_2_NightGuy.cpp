#include "stdafx.h"
#include "CAttack_2_NightGuy.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "CCamera_Free.h"
#include "Physics.h"

#include "CUser.h"

#include "CScript_FollowCam.h"
#include "CEffect.h"
#include "CUnit_Player.h"
#include "CState_Manager.h"
#include "CHit_Ground.h"
#include "CBlasted_Ground.h"
#include "CEffects_Factory.h"
#include "CNavigation.h"

#include "Model.h"
#include "CEveElephant.h"

#include "CCollider_Sphere.h"
CAttack_2_NightGuy::CAttack_2_NightGuy()
{
}

CAttack_2_NightGuy::~CAttack_2_NightGuy()
{
}

CAttack_2_NightGuy* CAttack_2_NightGuy::Create()
{
	CAttack_2_NightGuy* pInstance = new CAttack_2_NightGuy();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CAttack_2_NightGuy");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CAttack_2_NightGuy::Initialize()
{
	m_eAnimType = ANIM_EIGHTGATES;
	m_iAnimIndex = 9;
	m_eStateType = STATE_ATTACK_2_NIGHTGUY;

	m_vecAdjState.push_back(STATE_ATTACK_1_NIGHTGUY);
	m_vecAdjState.push_back(STATE_DASH_NIGHTGUY);

	m_fInterPolationTime = 0.05f;
	m_fStateChangableTime = 0.75f;
	m_fDelayTime = 0.5f;


	return S_OK;
}

void CAttack_2_NightGuy::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
	//CEffects_Factory::Get_Instance()->Create_MultiEffects(L"FireBallLaunch", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));


	if (pOwner->Get_PhysicsCom()->Get_Physics().bAir)
	{
		m_iAnimIndex = 2;
		pOwner->Get_PhysicsCom()->Set_Jump(0.f);
		pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 0.f;
		m_vecAdjState.push_back(STATE_LAND_NIGHTGUY);
		m_vecAdjState.push_back(STATE_DOUBLEJUMP_NIGHTGUY);
		pOwner->Get_PhysicsCom()->Mul_Speed(0.5f);
	}
	else
	{
		m_iAnimIndex = 9;
		m_vecAdjState.push_back(STATE_JUMP_NIGHTGUY);
		m_vecAdjState.push_back(STATE_IDLE_NIGHTGUY);

	}

	CUnit* pTargetUnit = pOwner->Get_TargetUnit();

	if (pTargetUnit)
	{
		_float4 vTargetPos = pTargetUnit->Get_Transform()->Get_World(WORLD_POS);
		_float4 vMyPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
		_float4 vTargetDir = vTargetPos - vMyPos;
		pOwner->Get_Transform()->Set_LerpLook(vTargetDir.Normalize(), 0.2f);
		pOwner->Get_Transform()->Set_Look(vTargetDir);
		pOwner->Get_Transform()->Make_WorldMatrix();
	}
	else
	{
		_float4 vLook = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
		vLook.y = 0.f;
		vLook.Normalize();
		pOwner->Get_Transform()->Set_LerpLook(vLook, 0.2f);
		pOwner->Get_Transform()->Set_Look(vLook);
		pOwner->Get_Transform()->Make_WorldMatrix();
	}


	__super::Enter(pOwner, pAnimator);

}

STATE_TYPE CAttack_2_NightGuy::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Is_CurAnimFinished() &&
		pOwner->Get_PhysicsCom()->Get_Physics().bAir)
	{
		return STATE_FALL_NIGHTGUY;
	}

	return __super::Tick(pOwner, pAnimator);

}

STATE_TYPE CAttack_2_NightGuy::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* Player가 Dash으로 오는 조건
	1. 키눌렸을때
	스킬
	스킬 쿨타임도 확인하고
	스킬 챠크라 게이지도 확인하고

	*/
	if (CUser::Get_Instance()->Get_LastKey() == KEY::LBUTTON)
	{
		return m_eStateType;
	}


	return STATE_END;
}

void CAttack_2_NightGuy::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
	CUser::Get_Instance()->Get_FollowCam()->Start_ShakingCamera(0.1f, 0.3f);
	static_cast<CHit_Ground*>(CState_Manager::Get_Instance()->Get_State(STATE_HIT_GROUND_ENEMY))->On_HitGround(
		CHit_Ground::SPINBLOW, 13.f, 10.f, 10.f);


	CUnit* pTargetUnit = pOwner->Get_TargetUnit();
	_float4 vFireTargetDir;

	if (pTargetUnit)
	{
		_float4 vTargetPos = pTargetUnit->Get_Transform()->Get_World(WORLD_POS);
		vTargetPos.y += 0.6f;
		_float4 vMyPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
		_float4 vTargetDir = vFireTargetDir = vTargetPos - vMyPos;
		vTargetDir.y = 0.f;
		pOwner->Get_Transform()->Set_LerpLook(vTargetDir.Normalize(), 0.2f);
		pOwner->Get_Transform()->Set_Look(vTargetDir);
		pOwner->Get_Transform()->Make_WorldMatrix();
	}
	else
	{
		_float4 vLook = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
		vLook.y = 0.f;
		vLook.Normalize();
		vFireTargetDir = vLook;
		pOwner->Get_Transform()->Set_LerpLook(vLook, 0.2f);
		pOwner->Get_Transform()->Set_Look(vLook);
		pOwner->Get_Transform()->Make_WorldMatrix();
	}

	if (pOwner->Get_PhysicsCom()->Get_Physics().bAir)
	{
		pOwner->Get_PhysicsCom()->Set_Jump(4.f);
		pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 9.8f;
	}
	_float4 vPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
	vPos.y += 1.f;

	_float4 vTargetPos = vPos + vFireTargetDir.Normalize() * 200.f;
	CGameObject* pBall = CEffects_Factory::Get_Instance()->Create_Effects(HASHCODE(CEveElephant), vPos, vTargetPos);

	GET_COMPONENT_FROM(pBall, CNavigation)->Set_StartCell(GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());

	CEffects_Factory::Get_Instance()->Create_ScreenEffects(pBall);

	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"EightGatesLaunch", pOwner, vPos);


}

void CAttack_2_NightGuy::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	CAMERA_LERP(CAMERA_LERP_DEFAULT);

	if (pOwner->Get_PhysicsCom()->Get_Physics().fGravity < 9.8f)
	{
		pOwner->Get_PhysicsCom()->Set_Jump(0.f);
		pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 9.8f;
	}

}
