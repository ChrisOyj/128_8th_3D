#include "stdafx.h"
#include "CLaughingMonk.h"

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
#include "CCell.h"
#include "Model.h"
#include "Functor.h"
#include "CLaughingMonkEffect.h"


CLaughingMonk::CLaughingMonk()
{
}

CLaughingMonk::~CLaughingMonk()
{
}

CLaughingMonk* CLaughingMonk::Create()
{
	CLaughingMonk* pInstance = new CLaughingMonk();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CLaughingMonk");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CLaughingMonk::Initialize()
{
	m_eAnimType = ANIM_SKILL_DEFEND;
	m_iAnimIndex = 77;
	m_eStateType = STATE_LAUGHINGMONK_ENEMY;

	//멀어졌으면 run
	m_vecAdjState.push_back(STATE_RUN_ENEMY);
	m_vecAdjState.push_back(STATE_JUMP_ENEMY);

	//공격범위 안이면 공격, 대쉬
	m_vecAdjState.push_back(STATE_DASH_ENEMY);
	m_vecAdjState.push_back(STATE_HANDATTACK_2_ENEMY);

	m_fAnimSpeed = 1.f;
	m_fInterPolationTime = 0.1f;
	m_fStateChangableTime = 30.f;
	m_fDelayTime = 1.f;

	/* For Attack */
	m_fSpeed = 0.f;
	m_fJumpPower = 13.f;
	m_fKnockbackPower = 10.f;
	m_eHitStateType = STATE_HIT_GROUND_PLAYER;
	m_iHitType = CHit_Ground::SPINBLOW;
	m_iColliderType = CUnit::PARTS_END;


	return S_OK;
}

void CLaughingMonk::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
	CFunctor::Play_Sound(L"Eff_SkillStart", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

	_float4 vPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
	list<CGameObject*> EffectsList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"CJ_MaxCharge", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));

	LIGHTDESC			LightDesc;

	LightDesc.eType = tagLightDesc::TYPE_POINT;

	LightDesc.fRange = 4.f;

	LightDesc.vDiffuse = _float4(0.1f, 0.2f, 0.6f, 1.f);
	LightDesc.vAmbient = _float4(0.3f, 0.3f, 0.3f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.pOwner = EffectsList.back();
	LightDesc.vOffset.y = 0.5f;

	GAMEINSTANCE->Add_Light(LightDesc);

	if (pOwner->Get_SkillType() == CUnit::LAUGHINGMONK)
	{
		CFunctor::Play_Sound(L"Voice_LaughingMonk", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));
		pOwner->On_Use(CUnit::SKILL1);

	}
	else
	{
		pOwner->On_Use(CUnit::SKILL2);
		CFunctor::Play_Sound(L"Voice_Boss_StrongAttack", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));

	}

	__super::Enter(pOwner, pAnimator);

}

STATE_TYPE CLaughingMonk::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Is_CurAnimFinished())
		m_fStateChangableTime = 0.f;

	return CState::Tick(pOwner, pAnimator);

}

STATE_TYPE CLaughingMonk::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* Player가 Dash으로 오는 조건
	1. 키눌렸을때
	스킬
	스킬 쿨타임도 확인하고
	스킬 챠크라 게이지도 확인하고

	*/
	if (pOwner->Can_Use(CUnit::SKILL1) && 
		pOwner->Get_SkillType() == CUnit::LAUGHINGMONK)
	{
		return m_eStateType;
	}

	if (pOwner->Can_Use(CUnit::SKILL2) &&
		pOwner->Get_SkillType2() == CUnit::LAUGHINGMONK)
	{
		return m_eStateType;
	}


	return STATE_END;
}

void CLaughingMonk::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
	//__super::OnExecute(pOwner, pAnimator);

	CUser::Get_Instance()->Get_FollowCam()->Start_ShakingCamera(0.04f, 0.2f);

	CUnit* pTargetUnit = pOwner->Get_TargetUnit();

	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"LaughingMonkText2"), pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));

	if (pTargetUnit)
	{

		_float4 vTargetPos = pTargetUnit->Get_Transform()->Get_World(WORLD_POS);
		_float4 vMyPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
		_float4 vTargetDir = vTargetPos - vMyPos;
		vTargetDir.y = 0.f;

		pOwner->Get_Transform()->Set_LerpLook(vTargetDir.Normalize(), 0.2f);
		pOwner->Get_Transform()->Set_Look(vTargetDir);
		pOwner->Get_Transform()->Make_WorldMatrix();


		CGameObject* pBall = CEffects_Factory::Get_Instance()->Create_Effects(HASHCODE(CLaughingMonkEffect), pTargetUnit, vTargetPos);
		CCell* pCell = GET_COMPONENT_FROM(pTargetUnit, CNavigation)->Get_CurCell();
		static_cast<CLaughingMonkEffect*>(pBall)->Set_Cell(pCell);

		CFunctor::Play_Sound(L"Eff_WallCreate", CHANNEL_EFFECTS, pBall->Get_Transform()->Get_MyWorld(WORLD_POS));
		CFunctor::Play_Sound(L"Eff_Summon", CHANNEL_EFFECTS, pBall->Get_Transform()->Get_MyWorld(WORLD_POS));

		if (pTargetUnit->Get_PhysicsCom()->Get_Physics().bAir)
		{
			vTargetPos.y = pCell->Get_Point(CCell::POINT_A).m128_f32[1];
			pBall->Get_Transform()->Set_World(WORLD_POS, vTargetPos);
			pBall->Get_Transform()->Make_WorldMatrix();
		}

	}

	

	//타겟한테 손 생성

}

void CLaughingMonk::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
}
