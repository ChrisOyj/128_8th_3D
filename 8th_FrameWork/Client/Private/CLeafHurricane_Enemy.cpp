#include "stdafx.h"
#include "CLeafHurricane_Enemy.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "CCamera_Free.h"
#include "Physics.h"

#include "CUser.h"

#include "CScript_FollowCam.h"


#include "CState_Manager.h"
#include "CHit_Ground.h"
#include "CBlasted_Ground.h"
#include "CEffects_Factory.h"
#include "CNavigation.h"

#include "CUnit_Player.h"

#include "Functor.h"

CLeafHurricane_Enemy::CLeafHurricane_Enemy()
{
}

CLeafHurricane_Enemy::~CLeafHurricane_Enemy()
{
}

CLeafHurricane_Enemy* CLeafHurricane_Enemy::Create()
{
	CLeafHurricane_Enemy* pInstance = new CLeafHurricane_Enemy();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CLeafHurricane_Enemy");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CLeafHurricane_Enemy::Initialize()
{
	m_eAnimType = ANIM_SKILL_ATTACK;
	m_iAnimIndex = 39;
	m_eStateType = STATE_LEAFHURRICANE_ENEMY;

	m_vecAdjState.push_back(STATE_LEAFHURRICANE_ATTACK_ENEMY);

	m_fInterPolationTime = 0.05f;
	m_fStateChangableTime = 0.25;
	m_fDelayTime = 10.f;


	return S_OK;
}

void CLeafHurricane_Enemy::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
	CFunctor::Play_Sound(L"Eff_SkillStart", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
	CFunctor::Play_Sound(L"Eff_Dash", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));


	pOwner->On_Use(CUnit::SKILL1);

	if (pOwner->Get_Status().fMaxHP > 200.f)
		CFunctor::Play_Sound(L"Voice_Boss_StrongAttack", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));


	CPhysics* pPhysicsCom = pOwner->Get_PhysicsCom();
	pPhysicsCom->Set_MaxSpeed(pOwner->Get_Status().fDashSpeed + 10.f);
	CUnit* pTargetUnit = pOwner->Get_TargetUnit();

	if (pTargetUnit)
	{
		_float4 vTargetPos = pTargetUnit->Get_Transform()->Get_World(WORLD_POS);
		_float4 vMyPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
		_float4 vTargetDir = vTargetPos - vMyPos;
		vTargetDir.y = 0.f;
		pOwner->Get_Transform()->Set_LerpLook(vTargetDir.Normalize(), 0.3f);
		pOwner->Get_Transform()->Set_Look(vTargetDir);
		pOwner->Get_Transform()->Make_WorldMatrix();
	}

	if (!pOwner->Get_PhysicsCom()->Get_Physics().bAir)
		CEffects_Factory::Get_Instance()->Create_LandingEffects(pOwner->Get_Transform()->Get_World(WORLD_POS), GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Dash_Effects_Ground", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));

	__super::Enter(pOwner, pAnimator);

}

STATE_TYPE CLeafHurricane_Enemy::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	CUnit* pTargetUnit = pOwner->Get_TargetUnit();
	CPhysics* pPhysicsCom = pOwner->Get_PhysicsCom();

	if (pTargetUnit)
	{
		_float4 vTargetPos = pTargetUnit->Get_Transform()->Get_World(WORLD_POS);
		_float4 vMyPos = pOwner->Get_Transform()->Get_World(WORLD_POS);

		_float4 vTargetDir = vTargetPos - vMyPos;

		vTargetDir.y = 0.f;
		vTargetPos -= vTargetDir * 0.5f;

		vTargetDir = vTargetPos - vMyPos;
		_float fLength = vTargetDir.Length();

		if (fLength < 0.3f)
		{
			return m_vecAdjState.front();
		}
		pPhysicsCom->Set_MaxSpeed(pOwner->Get_Status().fDashSpeed + 15.f);
		pPhysicsCom->Set_Accel(150.f);
		pPhysicsCom->Set_Dir(vTargetDir);

		if (pTargetUnit->Get_PhysicsCom()->Get_Physics().bAir)
		{
			pPhysicsCom->Set_Jump(0.f);
			pPhysicsCom->Get_Physics().fGravity = 0.f;
		}

	}
	else
	{
		pPhysicsCom->Set_Dir(pOwner->Get_Transform()->Get_World(WORLD_LOOK));
		pPhysicsCom->Set_Accel(100.f);

	}


	return __super::Tick(pOwner, pAnimator);

}

STATE_TYPE CLeafHurricane_Enemy::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* Player가 Dash으로 오는 조건
	1. 키눌렸을때
	스킬
	스킬 쿨타임도 확인하고
	스킬 챠크라 게이지도 확인하고

	*/
	_float4 vCurPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
	_float4 vTargetPos = pOwner->Get_TargetUnit()->Get_Transform()->Get_World(WORLD_POS);
	vTargetPos.y = 0.f;
	vCurPos.y = 0.f;
	_float4 vDir = vTargetPos - vCurPos;
	_float fTargetLength = vDir.Length();


	if (fTargetLength < 8.f)
	{
		if (pOwner->Can_Use(CUnit::SKILL1) && pOwner->Get_SkillType() == CUnit::LEAFHURRICANE)
		{
			return m_eStateType;
		}
	}
	



	return STATE_END;
}

void CLeafHurricane_Enemy::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 9.8f;

}
