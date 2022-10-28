#include "stdafx.h"
#include "CRasenSuriken.h"

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

#include "CCollider_Sphere.h"
#include "CRasenShurikenEffect.h"

#include "Functor.h"

CRasenSuriken::CRasenSuriken()
{
}

CRasenSuriken::~CRasenSuriken()
{
}

CRasenSuriken* CRasenSuriken::Create()
{
	CRasenSuriken* pInstance = new CRasenSuriken();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CRasenSuriken");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CRasenSuriken::Initialize()
{
	m_eAnimType = ANIM_SKILL_ATTACK;
	m_iAnimIndex = 64;
	m_eStateType = STATE_RASENSURIKEN_PLAYER;

	m_vecAdjState.push_back(STATE_DASH_PLAYER);
	m_vecAdjState.push_back(STATE_IDLE_PLAYER);

	m_fInterPolationTime = 0.1f;
	m_fStateChangableTime = 99.f;
	m_fDelayTime = 20.f;

	m_iFirstExecuteFrame = 10;
	m_iSecondExecuteFrame = 73;
	m_iStateChangableFrame = 85;

	m_wstrSoundKey = L"Eff_RasenShuriken_Loop";
	m_fSoundLoopTime = 0.35f;

	return S_OK;
}

void CRasenSuriken::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
	CFunctor::Play_Sound(L"Eff_SkillStart", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
	CFunctor::Play_Sound(L"Voice_Man_Rasenshuriken", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));

	list<CGameObject*> EffectsList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"KamuiEnter", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));

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


	if (pOwner->Get_PhysicsCom()->Get_Physics().bAir)
	{
		m_iAnimIndex = 28;
		pOwner->Get_PhysicsCom()->Set_Jump(0.f);
		pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 0.f;
		m_vecAdjState.push_back(STATE_LAND_PLAYER);
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

STATE_TYPE CRasenSuriken::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	_uint iCurFrame = pAnimator->Get_CurAnimFrame();

	if (!m_bFirstExecuted && iCurFrame == m_iFirstExecuteFrame)
	{
		m_bFirstExecuted = true;
		FirstExecute(pOwner, pAnimator);
	}
	else if (!m_bExecuted && iCurFrame == m_iSecondExecuteFrame)
	{
		OnExecute(pOwner, pAnimator);
	}
	else if (iCurFrame >= m_iStateChangableFrame)
	{
		m_fStateChangableTime = 0.f;
	}

	return __super::Tick(pOwner, pAnimator);

}

STATE_TYPE CRasenSuriken::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* Player가 Dash으로 오는 조건
	1. 키눌렸을때
	스킬
	스킬 쿨타임도 확인하고
	스킬 챠크라 게이지도 확인하고

	*/
	if (CUser::Get_Instance()->Get_LastKey() == KEY::R &&
		CUser::Get_Instance()->Can_UseSkill(2) &&
		PLAYER->Is_Hand())
	{
		return m_eStateType;
	}


	return STATE_END;
}

void CRasenSuriken::FirstExecute(CUnit* pOwner, CAnimator* pAnimator)
{
	CUser::Get_Instance()->Get_FollowCam()->Start_ShakingCamera(0.08f, 0.3f);
	CAMERA_LERP(CAMERA_LERP_MINIZOOMRIGHT);
	CFunctor::Play_Sound(L"Eff_RasenganStart", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));


	//CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"RASENGANTEXT"), pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));

	m_pRasenShuriken = CEffects_Factory::Get_Instance()->Create_Effects(HASHCODE(CRasenShurikenEffect), pOwner, 
		GET_COMPONENT_FROM(pOwner, CModel)->Find_HierarchyNode("RightHand"));

	GET_COMPONENT_FROM(m_pRasenShuriken, CNavigation)->Set_StartCell(GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());

	

	if (pOwner->Get_PhysicsCom()->Get_Physics().bAir)
	{
		pOwner->Get_PhysicsCom()->Mul_Speed(0.3f);
	}
}

void CRasenSuriken::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
	CFunctor::Play_Sound(L"Eff_MoveStrong", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
	CFunctor::Play_Sound(L"Eff_ChakraJumpMaxCharge", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

	CUser::Get_Instance()->Get_FollowCam()->Start_ShakingCamera(0.05f, 0.2f);

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
	_float4 vTargetPos = vPos + vFireTargetDir.Normalize() * 50.f;
	
	static_cast<CRasenShurikenEffect*>(m_pRasenShuriken)->On_Shoot(pOwner->Get_TargetUnit(), vTargetPos);
	m_pRasenShuriken = nullptr;

	m_bExecuted = true;

}

void CRasenSuriken::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	CAMERA_LERP(CAMERA_LERP_DEFAULT);

	if (pOwner->Get_PhysicsCom()->Get_Physics().fGravity < 9.8f)
	{
		pOwner->Get_PhysicsCom()->Set_Jump(0.f);
		pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 9.8f;
	}

	if (m_pRasenShuriken)
		DISABLE_GAMEOBJECT(m_pRasenShuriken);

	m_pRasenShuriken = nullptr;

}
