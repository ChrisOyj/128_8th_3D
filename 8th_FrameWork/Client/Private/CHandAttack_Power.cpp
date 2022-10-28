#include "stdafx.h"
#include "CHandAttack_Power.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "CCamera_Free.h"
#include "Physics.h"

#include "CUser.h"

#include "CScript_FollowCam.h"


#include "CState_Manager.h"
#include "CHit_Ground.h"
#include "CHit_Air.h"
#include "CBlasted_Ground.h"
#include "CEffects_Factory.h"
#include "CScreenEffect.h"
#include "Functor.h"
#include "CUnit_Player.h"

CHandAttack_Power::CHandAttack_Power()
{
}

CHandAttack_Power::~CHandAttack_Power()
{
}

CHandAttack_Power* CHandAttack_Power::Create()
{
    CHandAttack_Power* pInstance = new CHandAttack_Power();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CHandAttack_Power");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

void CHandAttack_Power::OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos)
{

    if (eColType == COL_ENEMYHITBOX)
    {
        CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_FINISHATTACKUP);
        CEffects_Factory::Get_Instance()->Create_ScreenEffects(pOtherObj);
        CFunctor::Play_Sound(L"Eff_Hit_Hand", CHANNEL_EFFECTS, PLAYER->Get_Transform()->Get_World(WORLD_POS));

    }
}

HRESULT CHandAttack_Power::Initialize()
{
    m_eAnimType = ANIM_ATTACK_HAND;
    m_iAnimIndex = 63;
    m_eStateType = STATE_HANDATTACK_POWER_PLAYER;

    m_vecAdjState.push_back(STATE_IDLE_PLAYER);
    m_vecAdjState.push_back(STATE_DASH_PLAYER);
    m_vecAdjState.push_back(STATE_JUMP_PLAYER);
    m_vecAdjState.push_back(STATE_RASENSURIKEN_PLAYER);
    m_vecAdjState.push_back(STATE_RASENGAN);
    m_vecAdjState.push_back(STATE_KAMUI_PLAYER);
    m_vecAdjState.push_back(STATE_EIGHTGATES);
    m_vecAdjState.push_back(STATE_CHIDORI);
    m_vecAdjState.push_back(STATE_FIREBALL_PLAYER);
    m_vecAdjState.push_back(STATE_THROW_PLAYER);

    m_fInterPolationTime = 0.1f;
    m_fStateChangableTime = 0.8f;
    m_fDelayTime = 0.5f;
    m_fAnimSpeed = 1.5f;

    /* For Attack */
    m_fSpeed = 10.f;
    m_fJumpPower = 10.f;
    m_fKnockbackPower = 3.f;
    m_eHitStateType = STATE_HIT_AIR_ENEMY;
    m_iHitType = CHit_Air::GOUP;
    m_iColliderType = CUnit::LEFTFOOT;

    return S_OK;
}

void CHandAttack_Power::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->Get_PhysicsCom()->Set_MaxSpeed(pOwner->Get_Status().fShortDashSpeed);
    pOwner->Get_PhysicsCom()->Set_Speed(pOwner->Get_Status().fShortDashSpeed);

    

    __super::Enter(pOwner, pAnimator);

    _float4 vLook = pOwner->Get_Transform()->Get_MyWorld(WORLD_LOOK);
    pOwner->Get_PhysicsCom()->Set_Dir(vLook);

}

STATE_TYPE CHandAttack_Power::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_bColHit)
    {
        m_fMyTimeAcc += RealfDT;
        if (m_fMyTimeAcc > 0.3f)
        {
            CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_QUICKDEFAULT);
            m_bColHit = false;

        }
    }

    return __super::Tick(pOwner, pAnimator);
}

void CHandAttack_Power::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_DEFAULT);

    __super::Exit(pOwner, pAnimator);

}

STATE_TYPE CHandAttack_Power::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Dash으로 오는 조건
    1. 키눌렸을때
    스킬
    스킬 쿨타임도 확인하고
    스킬 챠크라 게이지도 확인하고

    */
    if (CUser::Get_Instance()->Get_LastKey() == KEY::RBUTTON)
    {
        return m_eStateType;
    }


    return STATE_END;
}

void CHandAttack_Power::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{

    CFunctor::Play_Sound(L"Eff_MoveStrong", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
    CFunctor::Play_Sound(L"Voice_Man_StrongAttack", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));
    __super::OnExecute(pOwner, pAnimator);

}
