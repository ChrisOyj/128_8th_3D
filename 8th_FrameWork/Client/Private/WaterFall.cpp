#include "stdafx.h"
#include "WaterFall.h"

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
#include "Texture.h"

#include "CCollider_Sphere.h"
#include "CRasenShurikenEffect.h"

#include "CWaterUlt.h"
#include "Functor.h"

#include "CCamera_Cinema.h"
CWaterFall::CWaterFall()
{
}

CWaterFall::~CWaterFall()
{
}

CWaterFall* CWaterFall::Create()
{
	CWaterFall* pInstance = new CWaterFall();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CWaterFall");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CWaterFall::Initialize()
{
	m_eAnimType = ANIM_SKILL_DEFEND;
	m_iAnimIndex = 122;
	m_eStateType = STATE_WATERFALL_PLAYER;

	m_vecAdjState.push_back(STATE_IDLE_PLAYER);

	m_fInterPolationTime = 0.1f;
	m_fStateChangableTime = 99.f;
	m_fDelayTime = 20.f;

	m_iFirstExecuteFrame = 15;
	m_iSecondExecuteFrame = 48;
	m_iStateChangableFrame = 125;

	//23 :번뜩


	return S_OK;
}

void CWaterFall::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
	CFunctor::Play_Sound(L"Eff_SkillStart", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
	CFunctor::Play_Sound(L"Voice_Man_WaterFall", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));

	pOwner->Disable_AllColliders();

	list<CGameObject*> EffectsList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"KamuiEnter", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));





	CUser::Get_Instance()->On_UseSkill(4);

	CUser::Get_Instance()->Start_SkillCinema(CCamera_Cinema::CINEMA_WATER);

	__super::Enter(pOwner, pAnimator);

}

STATE_TYPE CWaterFall::Tick(CUnit* pOwner, CAnimator* pAnimator)
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

STATE_TYPE CWaterFall::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* Player가 Dash으로 오는 조건
	1. 키눌렸을때
	스킬
	스킬 쿨타임도 확인하고
	스킬 챠크라 게이지도 확인하고

	*/
	if (CUser::Get_Instance()->Get_LastKey() == KEY::V &&
		CUser::Get_Instance()->Can_UseSkill(4) &&
		PLAYER->Is_Water())
	{
		return m_eStateType;
	}


	return STATE_END;
}

void CWaterFall::FirstExecute(CUnit* pOwner, CAnimator* pAnimator)
{

	m_bFirstExecuted = true;
}

void CWaterFall::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
	CFunctor::Play_Sound(L"Eff_WaterOut", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

	CFunctor::Play_Sound(L"Eff_WaterFlow", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

	_float4 vPos = pOwner->Get_Transform()->Get_World(WORLD_POS);

	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"WaterUlt", pOwner, vPos);

	//카메라에 씌우삐기
	m_EffectsList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"WaterCam", GAMEINSTANCE->Get_CurCam(), vPos);


	list<CGameObject*>	EnemyList = GAMEINSTANCE->Get_ObjGroup(GROUP_ENEMY);
	for (auto& elem : EnemyList)
	{
		if (!elem->Is_Disable())
		{
			if ((elem->Get_Transform()->Get_World(WORLD_POS) - vPos).Length() < 20.f)
			{
				static_cast<CUnit*>(elem)->Enter_State(STATE_STRUGGLE_ENEMY);
			}
		}
	}

	m_bExecuted = true;

}

void CWaterFall::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_BodyColliders();

	CAMERA_LERP(CAMERA_LERP_DEFAULT);

	if (pOwner->Get_PhysicsCom()->Get_Physics().fGravity < 9.8f)
	{
		pOwner->Get_PhysicsCom()->Set_Jump(0.f);
		pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 9.8f;
	}


}
