#include "stdafx.h"
#include "CRasengan.h"

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
#include "Functor.h"
CRasengan::CRasengan()
{
}

CRasengan::~CRasengan()
{
}

CRasengan* CRasengan::Create()
{
	CRasengan* pInstance = new CRasengan();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CRasengan");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CRasengan::Initialize()
{
	m_eAnimType = ANIM_SKILL_ATTACK;
	m_iAnimIndex = 55;
	m_eStateType = STATE_RASENGAN;

	m_vecAdjState.push_back(STATE_DASH_PLAYER);
	m_vecAdjState.push_back(STATE_JUMP_PLAYER);
	m_vecAdjState.push_back(STATE_RASENGAN_RUN);

	m_fInterPolationTime = 0.15f;
	m_fDelayTime = 100.f;

	m_eChannel = CHANNEL_EFFECTS;
	m_fSoundLoopTime = 0.45f;

	return S_OK;
}

void CRasengan::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
	CFunctor::Play_Sound(L"Eff_SkillStart", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
	CFunctor::Play_Sound(L"Eff_RasenganStart", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
	CFunctor::Play_Sound(L"Voice_Man_RasenganCharge", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));

	CAMERA_LERP(CAMERA_LERP_ZOOM_RIGHT);
	CUser::Get_Instance()->On_UseSkill(3);

	m_EffectsList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"RasenganStart", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
	static_cast<CEffect*>(m_EffectsList.front())->Set_EffectFlag(EFFECT_FOLLOWTARGET);

	static_cast<CEffect*>(m_EffectsList.back())->Set_RefBone(GET_COMPONENT_FROM(pOwner, CModel)->Find_HierarchyNode("RightHand"));
	static_cast<CEffect*>(m_EffectsList.back())->Set_OffsetPos(_float4(10.f, 10.f, 0.f, 1.f));

	__super::Enter(pOwner, pAnimator);

}

STATE_TYPE CRasengan::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	m_fLoopSoundAcc += fDT;

	if (m_fLoopSoundAcc > m_fSoundLoopTime)
	{
		m_fLoopSoundAcc = 0.f;
		CFunctor::Play_Sound(L"Eff_RasenganLoop", m_eChannel, pOwner->Get_Transform()->Get_World(WORLD_POS));

	}




	if (m_iAnimIndex == 55)
	{
		if (pAnimator->Is_CurAnimFinished())
		{
			m_iAnimIndex = 57;
			OnExecute(pOwner, pAnimator);
			__super::Enter(pOwner, pAnimator);
		}

		return STATE_END;
		
	}


	return __super::Tick(pOwner, pAnimator);

}

STATE_TYPE CRasengan::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* Player가 Dash으로 오는 조건
	1. 키눌렸을때
	스킬
	스킬 쿨타임도 확인하고
	스킬 챠크라 게이지도 확인하고

	*/
	if (CUser::Get_Instance()->Get_LastKey() == KEY::T &&
		CUser::Get_Instance()->Can_UseSkill(3) &&
		PLAYER->Is_Hand())
	{
		return m_eStateType;
	}


	return STATE_END;
}

void CRasengan::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"CJ_MaxCharge", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
	CFunctor::Play_Sound(L"Eff_ChakraJumpMaxCharge", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
	m_bExecuted = true;
}

void CRasengan::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	for (auto& elem : m_EffectsList)
		if (!elem->Is_Disable())
			static_cast<CEffect*>(elem)->Set_FadeOut();

	//front :: Loop Effect
	DISABLE_GAMEOBJECT(m_EffectsList.front());
}
