#include "stdafx.h"
#include "CBossChidori.h"

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

#include "CRaijinKunai.h"

#include "Model.h"
#include "CHit_Ground_Player.h"
#include "CState_Manager.h"
#include "Functor.h"
CBossChidori::CBossChidori()
{
}

CBossChidori::~CBossChidori()
{
}

CBossChidori* CBossChidori::Create()
{
	CBossChidori* pInstance = new CBossChidori();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CBossChidori");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CBossChidori::Initialize()
{
	m_eStateType = STATE_CHIDORI_ENEMY;

	//1. 백대쉬로 거리벌리기
	m_arrBossPhase[BACKDASH].iAnimType = ANIM_BASE;
	m_arrBossPhase[BACKDASH].iAnimIndex = 23;
	m_arrBossPhase[BACKDASH].fAnimSpeed = 1.2f;
	m_arrBossPhase[BACKDASH].fDuration = 0.75f;
	m_arrBossPhase[BACKDASH].fInterPolationTime = 0.1f;

	// 2. 차징
	m_arrBossPhase[CHARGESTART].iAnimType = ANIM_SKILL_SHOOT;
	m_arrBossPhase[CHARGESTART].iAnimIndex = 15;
	m_arrBossPhase[CHARGESTART].fAnimSpeed = 2.f;
	m_arrBossPhase[CHARGESTART].fDuration = 0.f;
	m_arrBossPhase[CHARGESTART].fInterPolationTime = 0.05f;

	m_arrBossPhase[CHARGELOOP].iAnimType = ANIM_SKILL_SHOOT;
	m_arrBossPhase[CHARGELOOP].iAnimIndex = 16;
	m_arrBossPhase[CHARGELOOP].fAnimSpeed = 4.f;
	m_arrBossPhase[CHARGELOOP].fDuration = 0.f;
	m_arrBossPhase[CHARGELOOP].fInterPolationTime = 0.f;

	//3. 상대위치와 내 위치 사이에 수리검 4개 뿌림
	m_arrBossPhase[THROW].iAnimType = ANIM_THROW;
	m_arrBossPhase[THROW].iAnimIndex = 19;
	m_arrBossPhase[THROW].fAnimSpeed = 1.f;
	m_arrBossPhase[THROW].fDuration = 0.75f;
	m_arrBossPhase[THROW].fInterPolationTime = 0.1f;
	m_arrBossPhase[THROW].fDelayTime = 0.5f;

	//4. 대쉬
	m_arrBossPhase[DASH].iAnimType = ANIM_SKILL_SHOOT;
	m_arrBossPhase[DASH].iAnimIndex = 5;
	m_arrBossPhase[DASH].fDuration = 99.f;
	m_arrBossPhase[DASH].fInterPolationTime = 0;


	//5. 찌르깅
	m_arrBossPhase[ATTACK].iAnimType = ANIM_SKILL_SHOOT;
	m_arrBossPhase[ATTACK].iAnimIndex = 13;
	m_arrBossPhase[ATTACK].fDuration = 0.f;
	m_arrBossPhase[ATTACK].fInterPolationTime = 0;


	return S_OK;
}

void CBossChidori::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
	m_iAnimIndex = m_arrBossPhase[m_eCurPhase].iAnimIndex;
	m_eAnimType = (ANIM_TYPE)m_arrBossPhase[m_eCurPhase].iAnimType;
	m_fInterPolationTime = m_arrBossPhase[m_eCurPhase].fInterPolationTime;
	m_fStateChangableTime = m_arrBossPhase[m_eCurPhase].fDuration;
	m_fDelayTime = m_arrBossPhase[m_eCurPhase].fDelayTime;


	pOwner->On_Use(CUnit::SKILL2);


	/* Owner의 Animator Set Idle로 */
	__super::Enter(pOwner, pAnimator);


	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysics = pOwner->Get_PhysicsCom();

	CTransform* pTargetTransform = pOwner->Get_TargetUnit()->Get_Transform();
	CPhysics* pTargetPhysics = pOwner->Get_TargetUnit()->Get_PhysicsCom();

	switch (m_eCurPhase)
	{
	case Client::CBossChidori::BACKDASH:
	{
		CFunctor::Play_Sound(L"Eff_SkillStart", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

		CUser::Get_Instance()->On_Dialog(7);

		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"KamuiEnter", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));


		_float4 vLook = pTargetTransform->Get_World(WORLD_POS) - pMyTransform->Get_World(WORLD_POS);
		vLook.y = 0.f;
		pMyTransform->Set_Look(vLook);
		pMyPhysics->Set_Dir(vLook * -1.f);
		pMyPhysics->Set_MaxSpeed(20.f);
		pMyPhysics->Set_SpeedasMax();
		pMyPhysics->Get_PhysicsDetail().fFrictionRatio = 0.4f;
		pOwner->Disable_AllColliders();
	}
		break;
	case Client::CBossChidori::CHARGESTART:
	{
		CFunctor::Play_Sound(L"Voice_Boss_ChidoriStart", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));
		CFunctor::Play_Sound(L"Eff_BossChidoriLoop", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

		pMyPhysics->Get_PhysicsDetail().fFrictionRatio = 1.f;

		m_EffectsList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"ChidoriStart", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
		m_EffectsList.push_front(CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"ChidoLoop"), pOwner));
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

		LIGHTDESC			LightDesc;

		LightDesc.eType = tagLightDesc::TYPE_POINT;

		LightDesc.fRange = 5.f;

		LightDesc.vDiffuse = _float4(0.1f, 0.3f, 1.f, 1.f);
		LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
		LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
		LightDesc.pOwner = pOwner;
		LightDesc.fLightTime = 2.f;
		GAMEINSTANCE->Add_Light(LightDesc);
	}
		

		break;
	case Client::CBossChidori::CHARGELOOP:
	{
		CFunctor::Play_Sound(L"Eff_ChidoriLoop", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

	}
		break;
	case Client::CBossChidori::THROW:
		DISABLE_GAMEOBJECT(m_EffectsList.front());
		m_EffectsList.pop_front();
		break;

	case Client::CBossChidori::DASH:
	{
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"BossAuraStart", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));

		CEffects_Factory::Get_Instance()->Create_LandingEffects(pOwner->Get_Transform()->Get_World(WORLD_POS), GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());
		CGameObject* pTemp = m_EffectsList.back();
		m_EffectsList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"ChidoriRun", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
		m_EffectsList.push_back(pTemp);

		LIGHTDESC			LightDesc;

		LightDesc.eType = tagLightDesc::TYPE_POINT;

		LightDesc.fRange = 5.f;

		LightDesc.vDiffuse = _float4(0.1f, 0.4f, 1.f, 1.f);
		LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
		LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
		LightDesc.pOwner = m_EffectsList.back();

		GAMEINSTANCE->Add_Light(LightDesc);

		m_fChidoriLoopAcc = 0.f;

		//속도랑 방향
		_float4 vLook = pTargetTransform->Get_World(WORLD_POS) - pMyTransform->Get_World(WORLD_POS);
		
		pMyPhysics->Set_Dir(vLook);
		pMyPhysics->Set_MaxSpeed(20.f);
		pMyPhysics->Set_Speed(20.f);
		pMyPhysics->Set_Jump(0.f);
		pMyPhysics->Get_Physics().fGravity = 0.f;


		vLook.y = 0.f;

		pMyTransform->Set_Look(vLook);

		break;
	}
	case Client::CBossChidori::ATTACK:
	{
		CFunctor::Play_Sound(L"Voice_Boss_Chidori", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));
		CFunctor::Play_Sound(L"Eff_Dash", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
		CFunctor::Play_Sound(L"Eff_ChidoriHit", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

		CEffects_Factory::Get_Instance()->Create_ScreenEffects(pOwner);

		DISABLE_GAMEOBJECT(m_EffectsList.back());

		pMyPhysics->Set_Jump(0.f);
		pMyPhysics->Get_Physics().fGravity = 9.8f;
		pMyPhysics->Set_Speed(20.f);
		pMyPhysics->Set_SpeedasMax();

		pMyPhysics->Get_PhysicsDetail().fFrictionRatio = 0.7f;


		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Dash_Effects_Ground", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"ChidoriRun", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
		list<CGameObject*> EffectsList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"ChidoriDistortion", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
		static_cast<CEffect*>(EffectsList.front())->Set_ColliderOn(3.5f, COL_ENEMYSKILL);
		CUser::Get_Instance()->Get_FollowCam()->Start_ShakingCamera(0.1f, 0.3f);

		_float4 vLook = pOwner->Get_Transform()->Get_World(WORLD_LOOK);
		vLook.y = 0.f;
		vLook.Normalize();
		static_cast<CHit_Ground_Player*>(CState_Manager::Get_Instance()->Get_State(STATE_HIT_GROUND_PLAYER))->On_HitGround(
			CHit_Ground_Player::CHIDORIHIT, vLook * -1.f, vLook, 5.f, 0.f, 10.f);

		pOwner->Get_Transform()->Set_Look(vLook);

	}
	break;
	default:
		break;
	}






	__super::Enter(pOwner, pAnimator);

}

STATE_TYPE CBossChidori::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	STATE_TYPE eType = STATE_END;


	m_fTimeAcc += fDT;

	//
	if (m_fStateChangableTime <= 0.f)
	{
		if (pAnimator->Is_CurAnimFinished())
		{
			if (m_eCurPhase == ATTACK)
			{
				if (pOwner->Get_PhysicsCom()->Get_Physics().bAir)
					return STATE_FALL_ENEMY;
				else
					return STATE_WALK_ENEMY;
			}


			//다음으로
			_int iCurPhase = m_eCurPhase;
			iCurPhase++;
			m_eCurPhase = (PHASE)iCurPhase;
			Enter(pOwner, pAnimator);
			return eType;
		}
	}
	else
	{
		if (m_fTimeAcc >= m_fStateChangableTime)
		{
			if (m_eCurPhase == DASH)
			{
				pOwner->Get_PhysicsCom()->Set_MaxSpeed(8.f);
				pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 9.8f;
				pOwner->Get_PhysicsCom()->Set_Jump(0.f);
				return STATE_FALL_ENEMY;
			}

			//다음으로
			_int iCurPhase = m_eCurPhase;
			iCurPhase++;
			m_eCurPhase = (PHASE)iCurPhase;
			Enter(pOwner, pAnimator);
			return eType;
		}
		
		
	}

	if (m_fTimeAcc >= m_fDelayTime)
	{
		if (m_eCurPhase != DASH)
		{
			//실행
			if (!m_bExecuted)
				OnExecute(pOwner, pAnimator);
		}
		
	}

	switch (m_eCurPhase)
	{
	case Client::CBossChidori::BACKDASH:
		break;
	case Client::CBossChidori::CHARGESTART:
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
		break;
	case Client::CBossChidori::CHARGELOOP:
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
		break;
	case Client::CBossChidori::THROW:


		break;

	case Client::CBossChidori::DASH:
	{
		//가까이 닿으면 찌르기
		_float4 vTargetPos = pOwner->Get_TargetUnit()->Get_Transform()->Get_World(WORLD_POS);
		_float4 vMyPos = pOwner->Get_Transform()->Get_World(WORLD_POS);

		_float fTemp = vTargetPos.y;
		vTargetPos.y = vMyPos.y;
		if ((vTargetPos - vMyPos).Length() < 4.f)
		{
			m_bExecuted = true;
		}

		vTargetPos.y = fTemp;

		if (!m_bExecuted)
		{
			m_fChidoriLoopAcc += fDT;
			if (m_fChidoriLoopAcc > m_fDashTime)
			{
				m_fChidoriLoopAcc = 0.f;
				OnExecute(pOwner, pAnimator);
			}
		}
		else
		{
			//쿠나이 다 쓴거임
			//플레이어 향해서 돌진
			

			_float4 vTargetDir = vTargetPos - vMyPos;

			vTargetPos -= vTargetDir * 0.5f;
			vTargetDir = vTargetPos - vMyPos;

			if (vTargetDir.Length() < 1.f)
			{
				_int iCurPhase = m_eCurPhase;
				iCurPhase++;
				m_eCurPhase = (PHASE)iCurPhase;
				Enter(pOwner, pAnimator);
				return eType;
			}

			CPhysics* pPhysicsCom = pOwner->Get_PhysicsCom();

			pPhysicsCom->Set_MaxSpeed(pOwner->Get_Status().fDashSpeed + 15.f);
			pPhysicsCom->Set_SpeedasMax();
			pPhysicsCom->Set_Accel(150.f);
			pPhysicsCom->Set_Dir(vTargetDir);


			vTargetDir.y = 0.f;
			pOwner->Get_Transform()->Set_World(WORLD_LOOK, vTargetDir);


		}

		

		for (auto& elem : m_EffectsList)
			if (elem->Is_Disable())
				ENABLE_GAMEOBJECT(elem);




	}
		break;

	default:
		break;
	}

	return eType;

}

STATE_TYPE CBossChidori::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* Player가 Dash으로 오는 조건
	1. 키눌렸을때
	스킬
	스킬 쿨타임도 확인하고
	스킬 챠크라 게이지도 확인하고

	*/

	if (pOwner->Get_Status().fMaxHP > 200.f &&
		pOwner->Can_Use(CUnit::SKILL2))
	{
		_int iRand = random(0, 2);
		if (iRand == 0)
			return m_eStateType;
	}


	return STATE_END;
}

void CBossChidori::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysics = pOwner->Get_PhysicsCom();

	CTransform* pTargetTransform = pOwner->Get_TargetUnit()->Get_Transform();
	CPhysics* pTargetPhysics = pOwner->Get_TargetUnit()->Get_PhysicsCom();

	switch (m_eCurPhase)
	{
	case Client::CBossChidori::BACKDASH:
		m_bExecuted = true;
		break;
	case Client::CBossChidori::CHARGESTART:
		m_bExecuted = true;

		break;
	case Client::CBossChidori::CHARGELOOP:
		m_bExecuted = true;
		break;
	case Client::CBossChidori::THROW:
		//펴창 던지기
		m_bExecuted = true;
		m_KunaiList.clear();
		{

			_float4 vLook = pTargetTransform->Get_World(WORLD_POS) - pMyTransform->Get_World(WORLD_POS);
			vLook.Normalize();
			pMyTransform->Set_LerpLook(vLook, 0.2f);
			
			//4방향으로 표창을 던져주면댐
			_float4 vShuriPos = pMyTransform->Get_World(WORLD_POS);
			vShuriPos.y += 1.f;

			_uint iNumKunai = 10;

			for (_uint i = 0; i < iNumKunai; ++i)
			{
				_float4 vFinalLook = _float4(frandom(-0.2f, 0.2f), frandom(-0.05f, 0.1f), frandom(-0.2f, 0.2f), 0.f);
				vFinalLook += vLook;
				vFinalLook.Normalize();
				
				_float4 vTargetPos = vShuriPos + vFinalLook * 150.f;

				m_KunaiList.push_back(CEffects_Factory::Get_Instance()->Create_Effects(HASHCODE(CRaijinKunai), vShuriPos, vTargetPos));
				GET_COMPONENT_FROM(m_KunaiList.back(), CPhysics)->Set_MaxSpeed(25.f);
			}
			
		}
		break;

	case Client::CBossChidori::DASH:

		//1. 위치를 쿠나이 위치로 갱신
		//2. 바라보는건 플레이어
		//3. 쿠나이 팝 프론트
		//4. 쿠나이 없으면 bExecute true

	{
		CFunctor::Play_Sound(L"Eff_Raisin", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

		_float4 vKunaiPos = m_KunaiList.front()->Get_Transform()->Get_World(WORLD_POS);
		DISABLE_GAMEOBJECT(m_KunaiList.front());
		m_KunaiList.pop_front();

		pMyTransform->Set_World(WORLD_POS, vKunaiPos);
		pMyTransform->Make_WorldMatrix();
		pMyPhysics->Get_Physics().bAir = true;

		_float4 vLook = pTargetTransform->Get_World(WORLD_POS) - pMyTransform->Get_World(WORLD_POS);
		vLook.Normalize();
		pMyTransform->Set_Look(vLook);

		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"RaijinEffect", pOwner, vKunaiPos);


		if (m_KunaiList.empty())
		{
			m_bExecuted = true;
			m_fStateChangableTime = 0.5f;
			m_fTimeAcc = 0.f;
		}


		
	}


		break;

	default:
		break;
	}
}

void CBossChidori::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_BodyColliders();
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;

	_float4 vLook = pOwner->Get_Transform()->Get_World(WORLD_LOOK);
	vLook.y = 0.f;
	pOwner->Get_Transform()->Set_Look(vLook);


	for (auto& elem : m_EffectsList)
		if (!elem->Is_Disable())
			DISABLE_GAMEOBJECT(elem);

	//GAMEINSTANCE->Pop_Light();

}
