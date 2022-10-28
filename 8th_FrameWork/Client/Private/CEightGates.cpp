#include "stdafx.h"
#include "CEightGates.h"

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
#include "CCamera_Cinema.h"
#include "CBlasted_Ground.h"
#include "CEffects_Factory.h"
#include "CNavigation.h"
#include "CCell.h"

#include "Model.h"
#include "Texture.h"

#include "CCollider_Sphere.h"
#include "CRasenShurikenEffect.h"

#include "CEightGatesDragon.h"
#include "CEightGatesEffects.h"

#include "Functor.h"

CEightGates::CEightGates()
{
}

CEightGates::~CEightGates()
{
}

CEightGates* CEightGates::Create()
{
	CEightGates* pInstance = new CEightGates();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CEightGates");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CEightGates::Initialize()
{
	m_eAnimType = ANIM_EIGHTGATES;
	m_iAnimIndex = 10;
	m_eStateType = STATE_EIGHTGATES;

	m_vecAdjState.push_back(STATE_IDLE_NIGHTGUY);

	m_fAnimSpeed = 0.9f;
	m_fInterPolationTime = 0.f;
	m_fStateChangableTime = 99.f;
	m_fDelayTime = 20.f;

	m_iFirstExecuteFrame = 26;
	m_iSecondExecuteFrame = 199;
	m_iStateChangableFrame = 10;

	//23 :번뜩


	return S_OK;
}

void CEightGates::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Disable_AllColliders();
	CFunctor::Play_Sound(L"Eff_SkillStart", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
	CFunctor::Play_Sound(L"Voice_Man_EightGates", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));
	CFunctor::Play_Sound(L"Eff_EightGates", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));


	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"KamuiEnter", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));

	CUser::Get_Instance()->Start_SkillCinema(4);

	__super::Enter(pOwner, pAnimator);

	vector<pair<_uint, MODEL_MATERIAL>>& vecMaterials = GET_COMPONENT_FROM(pOwner, CModel)->Get_Materials();

	for (auto& elem : vecMaterials)
	{
		if (elem.second.strName == "MI_CHR_Eyes_WThinL_01")
		{
			m_iOriginEyeIndex = elem.second.pTextures[1]->Get_CurTextureIndex();
			elem.second.pTextures[1]->Set_CurTextureIndex(EYE_EIGHTGATES);

			continue;
		}

		if (elem.second.strName == "MI_CHR_Eyes_WThinR_01")
		{
			elem.second.pTextures[1]->Set_CurTextureIndex(EYE_EIGHTGATES);

			break;
		}
	}

}

STATE_TYPE CEightGates::Tick(CUnit* pOwner, CAnimator* pAnimator)
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

STATE_TYPE CEightGates::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* Player가 Dash으로 오는 조건
	1. 키눌렸을때
	스킬
	스킬 쿨타임도 확인하고
	스킬 챠크라 게이지도 확인하고

	*/
	if (CUser::Get_Instance()->Get_LastKey() == KEY::V &&
		CUser::Get_Instance()->Can_UseSkill(4) &&
		PLAYER->Is_Hand())
	{
		return m_eStateType;
	}


	return STATE_END;
}

void CEightGates::FirstExecute(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Set_Style(CUnit::STYLE_NIGHTGUY);
	CUser::Get_Instance()->On_StyleChange(3);

	CFunctor::Play_Sound(L"Eff_FireBomb", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

	static_cast<CCamera_Cinema*>(GAMEINSTANCE->Get_CurCam())->Start_ShakingCamera(0.3f, 0.4f);

	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"EightGatesStart", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
	CEffects_Factory::Get_Instance()->Create_EightGatesLandingEffects(pOwner->Get_Transform()->Get_World(WORLD_POS), GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());


	CGameObject* pGroundBreak = CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"GroundBreak_C"), pOwner->Get_Transform()->Get_World(WORLD_POS));
	pGroundBreak->Get_Transform()->Set_Up(_float4(0.f, 1.f, 0.f, 0.f));

	//이거 disable로 나중에 꺼야함
	pGroundBreak = CEffects_Factory::Get_Instance()->Create_Effects(HASHCODE(CEightGatesEffects), pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));

	PLAYER->Set_EightGatesEffect(pGroundBreak);
}

void CEightGates::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{

	m_bExecuted = true;

}

void CEightGates::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_BodyColliders();

	CAMERA_LERP(CAMERA_LERP_DEFAULT);

	if (pOwner->Get_PhysicsCom()->Get_Physics().fGravity < 9.8f)
	{
		pOwner->Get_PhysicsCom()->Set_Jump(0.f);
		pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 9.8f;
	}


	vector<pair<_uint, MODEL_MATERIAL>>& vecMaterials = GET_COMPONENT_FROM(pOwner, CModel)->Get_Materials();

	for (auto& elem : vecMaterials)
	{
		if (elem.second.strName == "MI_CHR_Eyes_WThinL_01")
		{
			elem.second.pTextures[1]->Set_CurTextureIndex(m_iOriginEyeIndex);

			continue;
		}

		if (elem.second.strName == "MI_CHR_Eyes_WThinR_01")
		{
			elem.second.pTextures[1]->Set_CurTextureIndex(m_iOriginEyeIndex);

			break;
		}
	}

}
