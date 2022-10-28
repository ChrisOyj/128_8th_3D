#include "stdafx.h"
#include "CChidori.h"

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

#include "CCell.h"

CChidori::CChidori()
{
}

CChidori::~CChidori()
{
}

CChidori* CChidori::Create()
{
	CChidori* pInstance = new CChidori();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CChidori");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CChidori::Initialize()
{
	m_eAnimType = ANIM_SKILL_SHOOT;
	m_iAnimIndex = 15;
	m_eStateType = STATE_CHIDORI;

	m_vecAdjState.push_back(STATE_DASH_PLAYER);
	m_vecAdjState.push_back(STATE_JUMP_PLAYER);
	m_vecAdjState.push_back(STATE_CHIDORI_RUN);

	m_fInterPolationTime = 0.1f;
	m_fDelayTime = 100.f;

	m_eChannel = CHANNEL_EFFECTS;
	m_fSoundLoopTime = 0.35f;

	return S_OK;
}

void CChidori::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
	CFunctor::Play_Sound(L"Eff_SkillStart", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
	CFunctor::Play_Sound(L"Voice_Man_ChidoriCharge", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));

	CUser::Get_Instance()->On_UseSkill(3);

	m_EffectsList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"ChidoriStart", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));

	if (pOwner->Get_PhysicsCom()->Get_Physics().bAir)
	{
		m_iAnimIndex = 2;
		pOwner->Get_PhysicsCom()->Set_Jump(4.f);

		_float4 vLook = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
		vLook.y = 0.f;
		vLook.Normalize();
		pOwner->Get_Transform()->Set_LerpLook(vLook, 0.3f);
		pOwner->Get_Transform()->Set_Look(vLook);
		pOwner->Get_Transform()->Make_WorldMatrix();


	}
	else
	{
		m_EffectsList.push_back(CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"ChidoLoop"), pOwner));
		m_iAnimIndex = 15;

	}
	CAMERA_LERP(CAMERA_LERP_ZOOM_RIGHT);


	m_EffectsList.push_back(CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"ChidoBall"), pOwner));

	for (auto& elem : m_EffectsList)
	{
		static_cast<CEffect*>(elem)->Set_FadeInStartTime(frandom(0.f, 0.3f));
	}

	//static_cast<CEffect*>(m_EffectsList.front())->Set_EffectFlag(EFFECT_FOLLOWTARGET);

	//Chidori Ball
	static_cast<CEffect*>(m_EffectsList.back())->Set_RefBone(GET_COMPONENT_FROM(pOwner, CModel)->Find_HierarchyNode("LeftHand"));
	static_cast<CEffect*>(m_EffectsList.back())->Set_OffsetPos(_float4(10.f, 10.f, 0.f, 1.f));
	static_cast<CEffect*>(m_EffectsList.back())->Late_Tick();


	_float4 vChidoriPos = m_EffectsList.back()->Get_Transform()->Get_MyWorld(WORLD_POS);

	LIGHTDESC			LightDesc;

	LightDesc.eType = tagLightDesc::TYPE_POINT;

	LightDesc.fRange = 5.f;

	LightDesc.vDiffuse = _float4(0.1f, 0.4f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.pOwner = m_EffectsList.back();

	GAMEINSTANCE->Add_Light(LightDesc);

	/*LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = tagLightDesc::TYPE_POINT;
	LightDesc.fRange = 2.f;
	LightDesc.vPosition = vChidoriPos;
	LightDesc.vDiffuse = _float4(0.2f, 0.6f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	GAMEINSTANCE->Add_Light(LightDesc);*/



	

	__super::Enter(pOwner, pAnimator);

}

STATE_TYPE CChidori::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	m_fLoopSoundAcc += fDT;

	if (m_fLoopSoundAcc > m_fSoundLoopTime)
	{
		m_fLoopSoundAcc = 0.f;
		CFunctor::Play_Sound(L"Eff_ChidoriLoop", m_eChannel, pOwner->Get_Transform()->Get_World(WORLD_POS));

	}


	m_fChidoriLoopAcc += fDT;
	if (m_fChidoriLoopAcc > m_fChidoriLoopTime)
	{
		_float4 vChidoriPos = m_EffectsList.back()->Get_Transform()->Get_MyWorld(WORLD_POS);
		CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"ChidoriText"), vChidoriPos);

		if (!pOwner->Get_PhysicsCom()->Get_Physics().bAir)
		{
			vChidoriPos.y = pOwner->Get_Transform()->Get_World(WORLD_POS).y;
			CEffects_Factory::Get_Instance()->Create_LandingEffects(vChidoriPos, GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());
		}
		

		m_fChidoriLoopAcc = 0.f;
		for (auto& elem : m_EffectsList)
			ENABLE_GAMEOBJECT(elem);
	}

	//공중이 아닌데 공중 동작이면
	if (!pOwner->Get_PhysicsCom()->Get_Physics().bAir)
	{
		if (m_iAnimIndex <= 10)
		{
			if (GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell()->IsGrass())
			{
				CFunctor::Play_Sound(L"Env_Landing_Grass", CHANNEL_ENVIRONMENT, pOwner->Get_Transform()->Get_World(WORLD_POS));
			}
			else
				CFunctor::Play_Sound(L"Env_Landing_Ground", CHANNEL_ENVIRONMENT, pOwner->Get_Transform()->Get_World(WORLD_POS));

			CEffects_Factory::Get_Instance()->Create_LandingEffects(pOwner->Get_Transform()->Get_World(WORLD_POS), GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());


			m_eAnimType = ANIM_BASE;
			m_fTimeAcc = 0.f;
			m_fInterPolationTime = 0.f;
			m_iAnimIndex = 51;
			__super::Enter(pOwner, pAnimator);
			m_eAnimType = ANIM_SKILL_SHOOT;

			return STATE_END;

		}


	}


	switch (m_iAnimIndex)
	{
	case 2:
		//Start in Air
		if (pAnimator->Is_CurAnimFinished())
		{
			m_iAnimIndex = 3;
			m_fTimeAcc = 0.f;
			m_fInterPolationTime = 0.f;

			__super::Enter(pOwner, pAnimator);
		}

		return STATE_END;

	case 3:
		//Start in Air
		if (pAnimator->Is_CurAnimFinished())
		{
			m_iAnimIndex = 4;
			m_fTimeAcc = 0.f;
			m_fInterPolationTime = 0.f;

			__super::Enter(pOwner, pAnimator);
		}

		break;

	case 15:
		//Start in Air
		if (pAnimator->Is_CurAnimFinished())
		{
			m_iAnimIndex = 16;
			m_fTimeAcc = 0.f;
			m_fInterPolationTime = 0.f;

			__super::Enter(pOwner, pAnimator);
		}
		return STATE_END;


	case 16:
		//Ground 2nd
		if (pAnimator->Is_CurAnimFinished())
		{
			m_iAnimIndex = 17;
			m_fTimeAcc = 0.f;
			m_fInterPolationTime = 0.f;

			__super::Enter(pOwner, pAnimator);
		}

		break;

	case 51:
		//Land
		if (pAnimator->Is_CurAnimFinished())
		{
			m_eAnimType = ANIM_SKILL_SHOOT;
			m_iAnimIndex = 16;
			m_fTimeAcc = 0.f;
			m_fInterPolationTime = 0.05f;

			__super::Enter(pOwner, pAnimator);
		}

		break;

	default:
		break;
	}


	return __super::Tick(pOwner, pAnimator);

}

STATE_TYPE CChidori::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* Player가 Dash으로 오는 조건
	1. 키눌렸을때
	스킬
	스킬 쿨타임도 확인하고
	스킬 챠크라 게이지도 확인하고

	*/
	if (CUser::Get_Instance()->Get_LastKey() == KEY::T &&
		CUser::Get_Instance()->Can_UseSkill(3) &&
		PLAYER->Is_NinzaSword())
	{
		return m_eStateType;
	}


	return STATE_END;
}

void CChidori::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
	m_bExecuted = true;
}

void CChidori::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	for (auto& elem : m_EffectsList)
		if (!elem->Is_Disable())
			DISABLE_GAMEOBJECT(elem);

	CAMERA_LERP(CAMERA_LERP_DEFAULT);

	//GAMEINSTANCE->Pop_Light();

}
