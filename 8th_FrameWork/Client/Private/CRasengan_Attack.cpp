#include "stdafx.h"
#include "CRasengan_Attack.h"

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
CRasengan_Attack::CRasengan_Attack()
{
}

CRasengan_Attack::~CRasengan_Attack()
{
}

void CRasengan_Attack::OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos)
{
	if (eColType == COL_ENEMYHITBOX)
	{
		CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_FINISHATTACK);
	}
}

CRasengan_Attack* CRasengan_Attack::Create()
{
	CRasengan_Attack* pInstance = new CRasengan_Attack();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CRasengan_Attack");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CRasengan_Attack::Initialize()
{
	m_eAnimType = ANIM_SKILL_ATTACK;
	m_iAnimIndex = 53;
	m_eStateType = STATE_RASENGAN_ATTACK;

	m_vecAdjState.push_back(STATE_IDLE_PLAYER);
	m_vecAdjState.push_back(STATE_DASH_PLAYER);
	m_vecAdjState.push_back(STATE_JUMP_PLAYER);
	m_vecAdjState.push_back(STATE_GUARD_PLAYER);

	m_fInterPolationTime = 0.05f;
	m_fStateChangableTime = 0.5f;
	m_fDelayTime = 0.15f;

	/* For Attack */
	m_fColliderDisableTime = 0.2f;
	m_fSpeed = 12.f;
	m_fJumpPower = 6.f;
	m_fKnockbackPower = 15.f;
	m_eHitStateType = STATE_BLASTED_GROUND_ENEMY;
	m_iHitType = CHit_Ground::HEAD;
	m_iColliderType = CUnit::RIGHTHAND;
	m_fDamage = 20.f;


	return S_OK;
}

void CRasengan_Attack::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
	CFunctor::Play_Sound(L"Voice_Man_Rasengan", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));
	CFunctor::Play_Sound(L"Eff_RasenganHit", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

	if (!pOwner->Get_PhysicsCom()->Get_Physics().bAir)
		CEffects_Factory::Get_Instance()->Create_LandingEffects(pOwner->Get_Transform()->Get_World(WORLD_POS), GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Dash_Effects_Ground", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));



	__super::Enter(pOwner, pAnimator);

}

STATE_TYPE CRasengan_Attack::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (m_fTimeAcc + fDT >= m_fStateChangableTime)
	{
		if (pOwner->Get_PhysicsCom()->Get_Physics().bAir)
			return STATE_FALL_PLAYER;
	}

	return __super::Tick(pOwner, pAnimator);

}

STATE_TYPE CRasengan_Attack::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* Player가 Dash으로 오는 조건
	1. 키눌렸을때
	스킬
	스킬 쿨타임도 확인하고
	스킬 챠크라 게이지도 확인하고

	*/
	return STATE_END;
}

void CRasengan_Attack::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
	CUser::Get_Instance()->Get_FollowCam()->Start_ShakingCamera(0.08f, 0.4f);
	list<CGameObject*> Effectslist = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"RasenganAttack", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
	for (auto& elem : Effectslist)
	{
		static_cast<CEffect*>(elem)->Set_EffectFlag(EFFECT_FOLLOWTARGET);
	}

	static_cast<CEffect*>(Effectslist.back())->Set_ColliderOn(2.f, COL_PLAYERATTACK);
	static_cast<CEffect*>(Effectslist.back())->Set_EffectFlag(EFFECT_FOLLOWTARGET | EFFECT_COLLIDER);

	__super::OnExecute(pOwner, pAnimator);
}

void CRasengan_Attack::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
	CAMERA_LERP(CAMERA_LERP_DEFAULT);

}
