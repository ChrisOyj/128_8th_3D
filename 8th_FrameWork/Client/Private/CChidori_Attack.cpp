#include "stdafx.h"
#include "CChidori_Attack.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "CCamera_Free.h"
#include "Physics.h"

#include "CUser.h"

#include "CScript_FollowCam.h"

#include "CUnit_Player.h"
#include "CState_Manager.h"
#include "CHit_Ground.h"
#include "CBlasted_Ground.h"
#include "CEffects_Factory.h"
#include "CEffect.h"
#include "CNavigation.h"
#include "Functor.h"
CChidori_Attack::CChidori_Attack()
{
}

CChidori_Attack::~CChidori_Attack()
{
}

CChidori_Attack* CChidori_Attack::Create()
{
	CChidori_Attack* pInstance = new CChidori_Attack();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CChidori_Attack");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CChidori_Attack::Initialize()
{
	m_eAnimType = ANIM_SKILL_SHOOT;
	m_iAnimIndex = 13;
	m_eStateType = STATE_CHIDORI_ATTACK;

	m_vecAdjState.push_back(STATE_IDLE_PLAYER);
	m_vecAdjState.push_back(STATE_DASH_PLAYER);
	m_vecAdjState.push_back(STATE_JUMP_PLAYER);
	m_vecAdjState.push_back(STATE_GUARD_PLAYER);

	m_fInterPolationTime = 0.1f;
	m_fStateChangableTime = 0.6f;
	m_fDelayTime = 0.1f;

	/* For Attack */
	m_fColliderDisableTime = 0.1f;
	m_fSpeed = 20.f;
	m_fJumpPower = 0.f;
	m_fKnockbackPower = 5.f;
	m_eHitStateType = STATE_HIT_GROUND_ENEMY;
	m_iHitType = CHit_Ground::CHIDORIHIT;
	m_iColliderType = CUnit::PARTS_END;
	m_fDamage = 20.f;


	return S_OK;
}

void CChidori_Attack::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Disable_AllColliders();
	CFunctor::Play_Sound(L"Eff_ChidoriHit", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

	if (!pOwner->Get_PhysicsCom()->Get_Physics().bAir)
		CEffects_Factory::Get_Instance()->Create_LandingEffects(pOwner->Get_Transform()->Get_World(WORLD_POS), GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Dash_Effects_Ground", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));

	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"ChidoriRun", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));


	__super::Enter(pOwner, pAnimator);

}

STATE_TYPE CChidori_Attack::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (m_fTimeAcc + fDT >= m_fStateChangableTime)
	{
		if (pOwner->Get_PhysicsCom()->Get_Physics().bAir)
		{
			pOwner->Get_PhysicsCom()->Mul_Speed(0.5f);
			return STATE_FALL_PLAYER;

		}
	}

	return CState::Tick(pOwner, pAnimator);

}

STATE_TYPE CChidori_Attack::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	
	if (pAnimator->Is_CurAnimFinished())
		return m_eStateType;

	return STATE_END;
}

void CChidori_Attack::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
	CFunctor::Play_Sound(L"Eff_MoveStrong", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
	CFunctor::Play_Sound(L"Voice_Man_Chidori", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));
	CFunctor::Play_Sound(L"Eff_Electric", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

	CEffects_Factory::Get_Instance()->Create_ScreenEffects(pOwner);

	list<CGameObject*> EffectsList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"ChidoriDistortion", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));

	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
	//chido 4
	static_cast<CEffect*>(EffectsList.front())->Set_ColliderOn(4.f, COL_PLAYERATTACK);

	CUser::Get_Instance()->Get_FollowCam()->Start_ShakingCamera(0.1f, 0.3f);
	CAMERA_LERP(CAMERA_LERP_FINISHATTACKBACK);

	/*list<CGameObject*> Effectslist = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"RasenganAttack", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));

	for (auto& elem : Effectslist)
	{
		static_cast<CEffect*>(elem)->Set_EffectFlag(EFFECT_FOLLOWTARGET);
	}

	static_cast<CEffect*>(Effectslist.back())->Set_ColliderOn(2.f, COL_PLAYERATTACK);
	static_cast<CEffect*>(Effectslist.back())->Set_EffectFlag(EFFECT_FOLLOWTARGET | EFFECT_COLLIDER);*/

	__super::OnExecute(pOwner, pAnimator);
}

void CChidori_Attack::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_BodyColliders();


	__super::Exit(pOwner, pAnimator);
	CPhysics* pPhysicsCom = pOwner->Get_PhysicsCom();
	pPhysicsCom->Set_Jump(0.f);
	pPhysicsCom->Get_Physics().fGravity = 9.8f;
	CAMERA_LERP(CAMERA_LERP_DEFAULT);
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;

}
