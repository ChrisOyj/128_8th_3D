#include "stdafx.h"
#include "CBossKoto.h"

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

#include "CBossAura.h"
#include "CCamera_Cinema.h"

#include "CGoal.h"
#include "CTargetObject.h"

#include "Functor.h"

CBossKoto::CBossKoto()
{
}

CBossKoto::~CBossKoto()
{
}

CBossKoto* CBossKoto::Create()
{
	CBossKoto* pInstance = new CBossKoto();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CBossKoto");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CBossKoto::Initialize()
{
	m_eAnimType = ANIM_BASE;
	m_iAnimIndex = 23;
	m_eStateType = STATE_KOTOAMATSUKAMI;

	m_fAnimSpeed = 0.8f;
	m_fInterPolationTime = 0.1f;
	m_fStateChangableTime = 99.f;
	m_fDelayTime = 20.f;

	m_iFirstExecuteFrame = 47;
	m_iSecondExecuteFrame = 53;
	m_iStateChangableFrame = 10;

	m_vecAdjState.push_back(STATE_IDLE_ENEMY);

	//23 :¹ø¶à


	return S_OK;
}

void CBossKoto::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
	CFunctor::Play_Sound(L"Eff_SkillStart", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

	CTransform* pTargetTransform = pOwner->Get_TargetUnit()->Get_Transform();
	CTransform* pMyTransform = pOwner->Get_Transform();

	CPhysics* pMyPhysics = pOwner->Get_PhysicsCom();

	if (m_iAnimIndex == 23)
	{
		CFunctor::Play_Sound(L"Voice_Boss_Hit", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));


		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"KamuiEnter", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
		_float4 vLook = pTargetTransform->Get_World(WORLD_POS) - pMyTransform->Get_World(WORLD_POS);

		vLook.y = 0.f;
		pMyTransform->Set_Look(vLook);
		pMyPhysics->Set_Dir(vLook * -1.f);
		pMyPhysics->Set_MaxSpeed(20.f);
		pMyPhysics->Set_SpeedasMax();
		pMyPhysics->Get_PhysicsDetail().fFrictionRatio = 0.5f;
		pOwner->Disable_AllColliders();
	}
	else
	{
		

		pOwner->Disable_AllColliders();



		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"KamuiEnter", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));


		CUnit* pTargetUnit = pOwner->Get_TargetUnit();

		if (pTargetUnit)
		{
			_float4 vTargetPos = pTargetUnit->Get_Transform()->Get_World(WORLD_POS);
			_float4 vMyPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
			_float4 vTargetDir = vTargetPos - vMyPos;
			vTargetDir.y = 0.f;
			pOwner->Get_Transform()->Set_Look(vTargetDir);
			pOwner->Get_Transform()->Make_WorldMatrix();
		}

	}
	

	__super::Enter(pOwner, pAnimator);

}

STATE_TYPE CBossKoto::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (m_bTerm)
	{
		m_fTermAcc += fDT;

		if (m_fTermAcc >= 0.1f)
		{
			m_bTerm = false;

			Enter(pOwner, pAnimator);


		}

		return STATE_END;
	}



	if (m_eAnimType == ANIM_BASE)
	{
		if (pAnimator->Is_CurAnimFinished())
		{
			m_bTerm = true;


			CUser::Get_Instance()->Start_SkillCinema(CCamera_Cinema::CINEMA_KOTO);
			CUser::Get_Instance()->Disable_HUD();




			m_iAnimIndex = 0;
			m_eAnimType = ANIM_KOTO;

		}

		return STATE_END;
	}
	else
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

		if (pAnimator->Is_CurAnimFinished())
			return STATE_IDLE_ENEMY;

		return STATE_END;
	}


	

	return __super::Tick(pOwner, pAnimator);

}

STATE_TYPE CBossKoto::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return m_eStateType;
}

void CBossKoto::FirstExecute(CUnit* pOwner, CAnimator* pAnimator)
{
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"BossAuraStart", pOwner, pOwner->Get_Transform()->Get_MyWorld(WORLD_POS));
	static_cast<CCamera_Cinema*>(GAMEINSTANCE->Get_CurCam())->Start_ShakingCamera(0.15f, 0.3f);
	GAMEINSTANCE->Play_Sound(L"Voice_Boss_Sharingan", (CHANNEL_GROUP)CHANNEL_VOICE);


	pOwner->Set_NoSkill(true);
	pOwner->Get_TargetUnit()->Set_NoSkill(true);
	CUser::Get_Instance()->Get_BossAura()->On_StartFog();

	vector<pair<_uint, MODEL_MATERIAL>>& vecMaterials = GET_COMPONENT_FROM(pOwner, CModel)->Get_Materials();

	for (auto& elem : vecMaterials)
	{
		if (elem.second.strName == "MI_CHR_Eyes_WThinL_01")
		{
			m_iOriginEyeIndex = elem.second.pTextures[1]->Get_CurTextureIndex();
			elem.second.pTextures[1]->Set_CurTextureIndex(EYE_KOTO);

			continue;
		}

		if (elem.second.strName == "MI_CHR_Eyes_WThinR_01")
		{
			elem.second.pTextures[1]->Set_CurTextureIndex(EYE_KOTO);

			break;
		}

	}
}

void CBossKoto::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
	m_bExecuted = true;

}

void CBossKoto::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	CUser::Get_Instance()->On_Dialog(8);
	CFunctor::Play_Sound(L"Voice_Boss_KotoEnd", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));

	pOwner->Enable_BodyColliders();

	/*vector<pair<_uint, MODEL_MATERIAL>>& vecMaterials = GET_COMPONENT_FROM(pOwner, CModel)->Get_Materials();
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
	}*/

}
