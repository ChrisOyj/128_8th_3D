#include "stdafx.h"
#include "CNightGuy.h"

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
#include "CNightGuy_Run.h"
#include "Model.h"
#include "Texture.h"

#include "CCollider_Sphere.h"
#include "CRasenShurikenEffect.h"

#include "CEightGatesDragon.h"
#include "CEightGatesEffects.h"

#include "CCamera_Cinema.h"

#include "Functor.h"

CNightGuy::CNightGuy()
{
}

CNightGuy::~CNightGuy()
{
}

CNightGuy* CNightGuy::Create()
{
	CNightGuy* pInstance = new CNightGuy();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CNightGuy");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CNightGuy::Initialize()
{
	m_eAnimType = ANIM_EIGHTGATES;
	m_iAnimIndex = 6;
	m_eStateType = STATE_NIGHTGUY_PLAYER;

	m_vecAdjState.push_back(STATE_NIGHTGUYATTACK_PLAYER);

	m_fInterPolationTime = 0.05f;
	m_fStateChangableTime = 99.f;
	m_fDelayTime = 20.f;

	m_iFirstExecuteFrame = 10;
	m_iSecondExecuteFrame = 28;
	m_iStateChangableFrame = 45;

	//23 :번뜩


	return S_OK;
}

void CNightGuy::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
	CFunctor::Play_Sound(L"Eff_NightGuy", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
	CFunctor::Play_Sound(L"Voice_Man_NightGuy", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));

	static_cast<CEightGatesEffects*>(PLAYER->Get_EightGatesEffect())->On_UseNightGuy();


	pOwner->Disable_AllColliders();

	m_pRasenShuriken = CEffects_Factory::Get_Instance()->Create_Effects(HASHCODE(CEightGatesDragon), pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));

	CUnit* pTargetUnit = pOwner->Get_TargetUnit();

	CUser::Get_Instance()->On_UseSkill(4);

	if (pTargetUnit)
	{
		_float4 vTargetPos = pTargetUnit->Get_Transform()->Get_World(WORLD_POS);
		_float4 vMyPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
		_float4 vTargetDir = vTargetPos - vMyPos;
		vTargetDir.y = 0.f;
		pOwner->Get_Transform()->Set_Look(vTargetDir);
		pOwner->Get_Transform()->Make_WorldMatrix();
	}
	else
	{
		_float4 vLook = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
		vLook.y = 0.f;
		vLook.Normalize();
		pOwner->Get_Transform()->Set_Look(vLook);
		pOwner->Get_Transform()->Make_WorldMatrix();
	}

	CUser::Get_Instance()->Start_SkillCinema(5);
	static_cast<CCamera_Cinema*>(GAMEINSTANCE->Get_CurCam())->Start_ShakingCamera(0.05f, 0.2f);


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

	__super::Enter(pOwner, pAnimator);

}

STATE_TYPE CNightGuy::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	_uint iCurFrame = pAnimator->Get_CurAnimFrame();
	//Temp
	static_cast<CEightGatesEffects*>(PLAYER->Get_EightGatesEffect())->On_UseNightGuy();

	if (!m_bFirstExecuted && iCurFrame == m_iFirstExecuteFrame)
	{
		m_bFirstExecuted = true;
		FirstExecute(pOwner, pAnimator);
	}
	else if (!m_bExecuted && iCurFrame == m_iSecondExecuteFrame)
	{
		OnExecute(pOwner, pAnimator);
	}
	else if (iCurFrame >= m_iStateChangableFrame &&  (m_fStateChangableTime > 0.f))
	{
		pOwner->Get_PhysicsCom()->Set_Jump(6.f);
		static_cast<CEightGatesDragon*>(m_pRasenShuriken)->Start_Dragon();
		m_fStateChangableTime = 0.f;
	}

	return __super::Tick(pOwner, pAnimator);

}

STATE_TYPE CNightGuy::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* Player가 Dash으로 오는 조건
	1. 키눌렸을때
	스킬
	스킬 쿨타임도 확인하고
	스킬 챠크라 게이지도 확인하고

	*/
	if (CUser::Get_Instance()->Get_LastKey() == KEY::V &&
		CUser::Get_Instance()->Can_UseSkill(4) &&
		PLAYER->Is_NightGuy())
	{
		return m_eStateType;
	}


	return STATE_END;
}

void CNightGuy::FirstExecute(CUnit* pOwner, CAnimator* pAnimator)
{
	CFunctor::Play_Sound(L"Eff_FireBomb", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"EightGatesStart", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
	static_cast<CCamera_Cinema*>(GAMEINSTANCE->Get_CurCam())->Start_ShakingCamera(0.1f, 0.4f);

	CGameObject* pGroundBreak = CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"GroundBreak_B"), pOwner->Get_Transform()->Get_World(WORLD_POS));
	pGroundBreak->Get_Transform()->Set_Up(_float4(0.f, 1.f, 0.f, 0.f));

	CEffects_Factory::Get_Instance()->Create_EightGatesLandingEffects(pOwner->Get_Transform()->Get_World(WORLD_POS), GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());

}

void CNightGuy::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
	static_cast<CCamera_Cinema*>(GAMEINSTANCE->Get_CurCam())->Start_ShakingCamera(0.05f, 0.3f);

	/*CUnit* pTargetUnit = pOwner->Get_TargetUnit();

	if (pTargetUnit)
	{
		_float4 vTargetPos = pTargetUnit->Get_Transform()->Get_World(WORLD_POS);
		_float4 vMyPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
		_float4 vTargetDir = vTargetPos - vMyPos;
		vTargetDir.y = 0.f;
		pOwner->Get_Transform()->Set_LerpLook(vTargetDir.Normalize(), 0.2f);
		pOwner->Get_Transform()->Set_Look(vTargetDir);
		pOwner->Get_Transform()->Make_WorldMatrix();
	}*/


	m_bExecuted = true;

}

void CNightGuy::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

	CAMERA_LERP(CAMERA_LERP_NIGHTGUY);




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

	//static_cast<CNightGuy_Run*>(CState_Manager::Get_Instance()->Get_State(STATE_NIGHTGUYATTACK_PLAYER))->Set_Dragon(m_pRasenShuriken);

	//DISABLE_GAMEOBJECT(m_pRasenShuriken);

}
