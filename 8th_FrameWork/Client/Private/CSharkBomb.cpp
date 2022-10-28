#include "stdafx.h"
#include "CSharkBomb.h"

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
#include "CSharkBombEffect.h"
#include "Functor.h"

#include "CCollider_Sphere.h"
CSharkBomb::CSharkBomb()
{
}

CSharkBomb::~CSharkBomb()
{
}

CSharkBomb* CSharkBomb::Create()
{
	CSharkBomb* pInstance = new CSharkBomb();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CSharkBomb");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CSharkBomb::Initialize()
{
	m_eAnimType = ANIM_SKILL_DEFEND;
	m_iAnimIndex = 121;
	m_eStateType = STATE_SHARKBOMB_PLAYER;

	m_vecAdjState.push_back(STATE_DASH_PLAYER);
	m_vecAdjState.push_back(STATE_IDLE_PLAYER);

	m_fAnimSpeed = 1.2f;
	m_fInterPolationTime = 0.1f;

	m_fStateChangableTime = 1.5f;
	m_fDelayTime = 1.2f;


	return S_OK;
}

void CSharkBomb::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
	CFunctor::Play_Sound(L"Eff_SkillStart", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
	CFunctor::Play_Sound(L"Voice_Man_Shark", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));

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

	CUser::Get_Instance()->On_UseSkill(2);

	pOwner->Get_PhysicsCom()->Set_MaxSpeed(6.f);


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

	CAMERA_LERP(CAMERA_LERP_ZOOM_RIGHT);

	__super::Enter(pOwner, pAnimator);

}

STATE_TYPE CSharkBomb::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Is_CurAnimFinished() &&
		pOwner->Get_PhysicsCom()->Get_Physics().bAir)
	{
		return STATE_FALL_PLAYER;
	}

	return __super::Tick(pOwner, pAnimator);

}

STATE_TYPE CSharkBomb::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* Player가 Dash으로 오는 조건
	1. 키눌렸을때
	스킬
	스킬 쿨타임도 확인하고
	스킬 챠크라 게이지도 확인하고

	*/
	if (CUser::Get_Instance()->Get_LastKey() == KEY::R &&
		CUser::Get_Instance()->Can_UseSkill(2) &&
		PLAYER->Is_Water())
	{
		return m_eStateType;
	}


	return STATE_END;
}

void CSharkBomb::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
	CUser::Get_Instance()->Get_FollowCam()->Start_ShakingCamera(0.04f, 0.2f);
	CFunctor::Play_Sound(L"Eff_Shark", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));


	CAMERA_LERP(CAMERA_LERP_QUICKDEFAULT);

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
		pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 9.8f;
	}
	_float4 vPos = pOwner->Get_Transform()->Get_World(WORLD_POS);

	_float4 vTargetPos = vPos + vFireTargetDir.Normalize() * 200.f;
	CGameObject* pBall = CEffects_Factory::Get_Instance()->Create_Effects(HASHCODE(CSharkBombEffect), vPos, vTargetPos);

	GET_COMPONENT_FROM(pBall, CCollider_Sphere)->Set_ColIndex(COL_PLAYERSKILL);
	GET_COMPONENT_FROM(pBall, CNavigation)->Set_StartCell(GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());
	CEffects_Factory::Get_Instance()->Create_ScreenEffects(pBall);

}

void CSharkBomb::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	CAMERA_LERP(CAMERA_LERP_DEFAULT);

	if (pOwner->Get_PhysicsCom()->Get_Physics().fGravity < 9.8f)
	{
		pOwner->Get_PhysicsCom()->Set_Jump(0.f);
		pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 9.8f;
	}

}
