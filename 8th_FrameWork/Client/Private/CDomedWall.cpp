#include "stdafx.h"
#include "CDomedWall.h"

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

#include "CWoodWall.h"


CDomedWall::CDomedWall()
{
}

CDomedWall::~CDomedWall()
{
}

CDomedWall* CDomedWall::Create()
{
	CDomedWall* pInstance = new CDomedWall();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CDomedWall");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CDomedWall::Initialize()
{
	m_eAnimType = ANIM_SKILL_DEFEND;
	m_iAnimIndex = 84;
	m_eStateType = STATE_DOMEDWALL_ENEMY;

	//멀어졌으면 run
	m_vecAdjState.push_back(STATE_DOMEDWALLEND_ENEMY);

	m_fAnimSpeed = 1.f;
	m_fInterPolationTime = 0.1f;
	m_fStateChangableTime = 30.f;
	m_fDelayTime = 0.7f;

	/* For Attack */
	m_fSpeed = 0.f;
	m_fJumpPower = 13.f;
	m_fKnockbackPower = 10.f;
	m_eHitStateType = STATE_HIT_GROUND_PLAYER;
	m_iHitType = CHit_Ground::SPINBLOW;
	m_iColliderType = CUnit::PARTS_END;


	return S_OK;
}

void CDomedWall::Enter(CUnit* pOwner, CAnimator* pAnimator)
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

	pOwner->On_Use(CUnit::SKILL1);

	__super::Enter(pOwner, pAnimator);

}

STATE_TYPE CDomedWall::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Is_CurAnimFinished())
		m_fStateChangableTime = 0.f;

	return CState::Tick(pOwner, pAnimator);

}

STATE_TYPE CDomedWall::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* Player가 Dash으로 오는 조건
	1. 키눌렸을때
	스킬
	스킬 쿨타임도 확인하고
	스킬 챠크라 게이지도 확인하고

	*/
	if (pOwner->Can_Use(CUnit::SKILL1) && pOwner->Get_SkillType() == CUnit::DOMEDWALL)
	{
		return m_eStateType;
	}


	return STATE_END;
}

void CDomedWall::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
	//__super::OnExecute(pOwner, pAnimator);

	CFunctor::Play_Sound(L"Eff_DomeCreate", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
	CFunctor::Play_Sound(L"Voice_DomedWall", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));
	CFunctor::Play_Sound(L"Eff_Summon", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

	CUser::Get_Instance()->Get_FollowCam()->Start_ShakingCamera(0.04f, 0.2f);

	CUnit* pTargetUnit = pOwner->Get_TargetUnit();

	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"LaughingMonkText2"), pOwner->Get_Transform()->Get_World(WORLD_POS));

	if (pTargetUnit)
	{

		_float4 vTargetPos = pTargetUnit->Get_Transform()->Get_World(WORLD_POS);
		_float4 vMyPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
		_float4 vTargetDir = vTargetPos - vMyPos;
		vTargetDir.y = 0.f;
		pOwner->Get_Transform()->Set_LerpLook(vTargetDir.Normalize(), 0.2f);
		pOwner->Get_Transform()->Set_Look(vTargetDir);
		pOwner->Get_Transform()->Make_WorldMatrix();
	}

	CEffects_Factory::Get_Instance()->Create_Effects(HASHCODE(CWoodWall), pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));


	//타겟한테 손 생성

}

void CDomedWall::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Exit(pOwner, pAnimator);
}
