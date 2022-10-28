#include "stdafx.h"
#include "CBossFire.h"

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

#include "CRaijinKunai.h"

#include "Model.h"
#include "CHit_Ground_Player.h"
#include "CState_Manager.h"

#include "CCollider_Sphere.h"

#include "CFireBallEffect.h"

#include "Functor.h"

CBossFire::CBossFire()
{
}

CBossFire::~CBossFire()
{
}

CBossFire* CBossFire::Create()
{
	CBossFire* pInstance = new CBossFire();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CBossFire");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CBossFire::Initialize()
{
	m_eStateType = STATE_FIREBALL_ENEMY;

	//1. 백대쉬로 거리벌리기
	m_arrBossPhase[BACKDASH].iAnimType = ANIM_BASE;
	m_arrBossPhase[BACKDASH].iAnimIndex = 23;
	m_arrBossPhase[BACKDASH].fAnimSpeed = 1.2f;
	m_arrBossPhase[BACKDASH].fDuration = 0.75f;
	m_arrBossPhase[BACKDASH].fInterPolationTime = 0.1f;

	// 2. 점프
	m_arrBossPhase[JUMP].iAnimType = ANIM_BASE;
	m_arrBossPhase[JUMP].iAnimIndex = 43;
	m_arrBossPhase[JUMP].fAnimSpeed = 1.f;
	m_arrBossPhase[JUMP].fDuration = 0.4f;
	m_arrBossPhase[JUMP].fInterPolationTime = 0.f;

	m_arrBossPhase[FIRE].iAnimType = ANIM_SKILL_SHOOT;
	m_arrBossPhase[FIRE].iAnimIndex = 6;
	m_arrBossPhase[FIRE].fAnimSpeed = 1.f;
	m_arrBossPhase[FIRE].fDuration = 0.9f;
	m_arrBossPhase[FIRE].fInterPolationTime = 0.1f;
	m_arrBossPhase[FIRE].fDelayTime = 0.5f;


	return S_OK;
}

void CBossFire::Enter(CUnit* pOwner, CAnimator* pAnimator)
{

	m_iAnimIndex = m_arrBossPhase[m_eCurPhase].iAnimIndex;
	m_eAnimType = (ANIM_TYPE)m_arrBossPhase[m_eCurPhase].iAnimType;
	m_fInterPolationTime = m_arrBossPhase[m_eCurPhase].fInterPolationTime;
	m_fStateChangableTime = m_arrBossPhase[m_eCurPhase].fDuration;
	m_fDelayTime = m_arrBossPhase[m_eCurPhase].fDelayTime;

	pOwner->On_Use(CUnit::SKILL1);

	/* Owner의 Animator Set Idle로 */
	__super::Enter(pOwner, pAnimator);


	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysics = pOwner->Get_PhysicsCom();

	CTransform* pTargetTransform = pOwner->Get_TargetUnit()->Get_Transform();
	CPhysics* pTargetPhysics = pOwner->Get_TargetUnit()->Get_PhysicsCom();

	switch (m_eCurPhase)
	{
	case Client::CBossFire::BACKDASH:
	{
		CFunctor::Play_Sound(L"Eff_SkillStart", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
		CFunctor::Play_Sound(L"Eff_GroundDash", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

		CUser::Get_Instance()->On_Dialog(6);

		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"KamuiEnter", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
		_float4 vLook = pTargetTransform->Get_World(WORLD_POS) - pMyTransform->Get_World(WORLD_POS);
		vLook.y = 0.f;
		pMyTransform->Set_Look(vLook);
		pMyPhysics->Set_Dir(vLook * -1.f);
		pMyPhysics->Set_MaxSpeed(20.f);
		pMyPhysics->Set_SpeedasMax();
		pMyPhysics->Get_PhysicsDetail().fFrictionRatio = 0.5f;
		pOwner->Disable_AllColliders();
	}

	break;

	case Client::CBossFire::JUMP:
	{
		CFunctor::Play_Sound(L"Eff_Jump", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
		CFunctor::Play_Sound(L"Voice_Boss_FireBall", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));

		CEffects_Factory::Get_Instance()->Create_LandingEffects(pOwner->Get_Transform()->Get_World(WORLD_POS), GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());
		
		_float4 vLook = pTargetTransform->Get_World(WORLD_POS) - pMyTransform->Get_World(WORLD_POS);
		vLook.y = 0.f;
		pMyTransform->Set_Look(vLook);

		pMyPhysics->Set_Jump(8.f);

		pMyPhysics->Set_MaxSpeed(6.f);
		pMyPhysics->Get_PhysicsDetail().fFrictionRatio = 0.1f;
		pOwner->Disable_AllColliders();
	}
	break;

	case Client::CBossFire::FIRE:
	{
		CFunctor::Play_Sound(L"Eff_FireBallLaunch", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

		pMyPhysics->Mul_Speed(0.5f);
		pOwner->Get_PhysicsCom()->Set_Jump(0.f);
		pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 0.f;
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"FireBallLaunch", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));

	}
	break;


	default:
		break;
	}






	__super::Enter(pOwner, pAnimator);

}

STATE_TYPE CBossFire::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	STATE_TYPE eType = STATE_END;


	m_fTimeAcc += fDT;

	//
	if (m_fStateChangableTime <= 0.f)
	{
		if (m_eCurPhase == FIRE)
		{
			return STATE_FALL_ENEMY;
		}

		if (pAnimator->Is_CurAnimFinished())
		{
			//다음으로
			_int iCurPhase = m_eCurPhase;
			iCurPhase++;
			m_eCurPhase = (PHASE)iCurPhase;
			Enter(pOwner, pAnimator);
			return eType;
		}
	}
	else
	{
		



		if (m_fTimeAcc >= m_fStateChangableTime)
		{
			if (m_eCurPhase == FIRE)
			{
					if (m_iCurFireCount >= m_iMaxFireCount)
						return STATE_FALL_ENEMY;
					else
					{
						Enter(pOwner, pAnimator);
						return eType;
					}
			}

			//다음으로
			_int iCurPhase = m_eCurPhase;
			iCurPhase++;
			m_eCurPhase = (PHASE)iCurPhase;
			Enter(pOwner, pAnimator);
			return eType;
		}


	}

	if (m_fTimeAcc >= m_fDelayTime)
	{
		if (!m_bExecuted)
			OnExecute(pOwner, pAnimator);
	}

	switch (m_eCurPhase)
	{
	case Client::CBossFire::BACKDASH:
		break;
	case Client::CBossFire::JUMP:
		break;
	case Client::CBossFire::FIRE:
		break;
	default:
		break;
	}

	return eType;

}

STATE_TYPE CBossFire::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* Player가 Dash으로 오는 조건
	1. 키눌렸을때
	스킬
	스킬 쿨타임도 확인하고
	스킬 챠크라 게이지도 확인하고

	*/

	if (pOwner->Get_Status().fMaxHP > 200.f &&
		pOwner->Can_Use(CUnit::SKILL1))
	{
		_int iRand = random(0, 1);
		if (iRand == 0)
			return m_eStateType;
	}


	return STATE_END;
}

void CBossFire::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysics = pOwner->Get_PhysicsCom();

	CTransform* pTargetTransform = pOwner->Get_TargetUnit()->Get_Transform();
	CPhysics* pTargetPhysics = pOwner->Get_TargetUnit()->Get_PhysicsCom();

	switch (m_eCurPhase)
	{
	case Client::CBossFire::BACKDASH:
		m_bExecuted = true;
		break;
	case Client::CBossFire::JUMP:
		m_bExecuted = true;
		break;
	case Client::CBossFire::FIRE:
	{
		CUnit* pTargetUnit = pOwner->Get_TargetUnit();
		_float4 vFireTargetDir;

		if (pTargetUnit)
		{
			_float4 vTargetPos = pTargetUnit->Get_Transform()->Get_World(WORLD_POS);
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
			pOwner->Get_PhysicsCom()->Set_Speed(1.f);
			pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 9.8f;
		}
		_float4 vPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
		vPos.y += 1.f;

		_float4 vTargetPos = vPos + vFireTargetDir.Normalize() * 200.f;
		CGameObject* pBall = CEffects_Factory::Get_Instance()->Create_Effects(HASHCODE(CFireBallEffect), vPos, vTargetPos);

		GET_COMPONENT_FROM(pBall, CCollider_Sphere)->Set_ColIndex(COL_ENEMYTHROW);
		GET_COMPONENT_FROM(pBall, CNavigation)->Set_StartCell(GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());


		m_bExecuted = true;

		m_iCurFireCount++;


		break;
	}

	default:
		break;
	}
}

void CBossFire::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_BodyColliders();
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;


	//GAMEINSTANCE->Pop_Light();

}
