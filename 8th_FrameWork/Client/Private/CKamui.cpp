#include "stdafx.h"
#include "CKamui.h"

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

#include "CKamuiEffect.h"
#include "Functor.h"
CKamui::CKamui()
{
}

CKamui::~CKamui()
{
}

CKamui* CKamui::Create()
{
	CKamui* pInstance = new CKamui();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CKamui");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CKamui::Initialize()
{
	m_eAnimType = ANIM_SKILL_ATTACK;
	m_iAnimIndex = 36;
	m_eStateType = STATE_KAMUI_PLAYER;

	m_vecAdjState.push_back(STATE_IDLE_PLAYER);

	m_fInterPolationTime = 0.1f;
	m_fStateChangableTime = 99.f;
	m_fDelayTime = 20.f;

	m_iFirstExecuteFrame = 20;
	m_iSecondExecuteFrame = 40;
	m_iStateChangableFrame = 68;

	//23 :번뜩


	return S_OK;
}

void CKamui::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
	CFunctor::Play_Sound(L"Eff_SkillStart", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

	pOwner->Disable_AllColliders();

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



	CUser::Get_Instance()->On_UseSkill(4);

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
	else
	{
		_float4 vLook = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
		vLook.y = 0.f;
		vLook.Normalize();
		pOwner->Get_Transform()->Set_Look(vLook);
		pOwner->Get_Transform()->Make_WorldMatrix();
	}

	CUser::Get_Instance()->Start_SkillCinema(3);

	__super::Enter(pOwner, pAnimator);

}

STATE_TYPE CKamui::Tick(CUnit* pOwner, CAnimator* pAnimator)
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

STATE_TYPE CKamui::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* Player가 Dash으로 오는 조건
	1. 키눌렸을때
	스킬
	스킬 쿨타임도 확인하고
	스킬 챠크라 게이지도 확인하고

	*/
	if (CUser::Get_Instance()->Get_LastKey() == KEY::V &&
		CUser::Get_Instance()->Can_UseSkill(4) &&
		PLAYER->Is_NinzaSword())
	{
		return m_eStateType;
	}


	return STATE_END;
}

void CKamui::FirstExecute(CUnit* pOwner, CAnimator* pAnimator)
{
	CFunctor::Play_Sound(L"Voice_Kakashi_Kamui", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));

	vector<pair<_uint, MODEL_MATERIAL>>& vecMaterials = GET_COMPONENT_FROM(pOwner, CModel)->Get_Materials();

	for (auto& elem : vecMaterials)
	{
		if (elem.second.strName == "MI_CHR_Eyes_WThinL_01")
		{
			m_iOriginEyeIndex = elem.second.pTextures[1]->Get_CurTextureIndex();
			elem.second.pTextures[1]->Set_CurTextureIndex(EYE_KAMUI);

			break;
		}

	}

	//CUser::Get_Instance()->Get_FollowCam()->Start_ShakingCamera(0.08f, 0.3f);


	//CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"RASENGANTEXT"), pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));

	/*m_pRasenShuriken = CEffects_Factory::Get_Instance()->Create_Effects(HASHCODE(CRasenShurikenEffect), pOwner,
		GET_COMPONENT_FROM(pOwner, CModel)->Find_HierarchyNode("RightHand"));

	GET_COMPONENT_FROM(m_pRasenShuriken, CNavigation)->Set_StartCell(GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());*/
}

void CKamui::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
	CFunctor::Play_Sound(L"Eff_Kamui", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));


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
		vFireTargetDir = pOwner->Get_Transform()->Get_World(WORLD_LOOK);
	}

	_float4 vPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
	_float4 vTargetPos = vPos + vFireTargetDir.Normalize() * 10.f;


	CCell* pCell = nullptr;

	if (pTargetUnit)
	{
		m_pRasenShuriken = CEffects_Factory::Get_Instance()->Create_Effects(HASHCODE(CKamuiEffect), pTargetUnit, pTargetUnit->Get_Transform()->Get_World(WORLD_POS));

		if (GET_COMPONENT_FROM(pTargetUnit, CNavigation)->Get_TargetY() + 4.f > pTargetUnit->Get_Transform()->Get_World(WORLD_POS).y)
		{
			pCell = GET_COMPONENT_FROM(pTargetUnit, CNavigation)->Get_CurCell();
		}
	}
	else
	{
		m_pRasenShuriken = CEffects_Factory::Get_Instance()->Create_Effects(HASHCODE(CKamuiEffect), vTargetPos);

		if (GET_COMPONENT_FROM(pOwner, CNavigation)->Get_TargetY() + 4.f > pOwner->Get_Transform()->Get_World(WORLD_POS).y)
		{
			pCell = GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell();
		}
	}

	static_cast<CKamuiEffect*>(m_pRasenShuriken)->Set_Cell(pCell);

	m_bExecuted = true;

}

void CKamui::Exit(CUnit* pOwner, CAnimator* pAnimator)
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
			break;
		}

	}

}
