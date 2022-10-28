#include "stdafx.h"
#include "CHandAttack_Dash.h"

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
#include "Functor.h"
CHandAttack_Dash::CHandAttack_Dash()
{
}

CHandAttack_Dash::~CHandAttack_Dash()
{
}

CHandAttack_Dash* CHandAttack_Dash::Create()
{
    CHandAttack_Dash* pInstance = new CHandAttack_Dash();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CHandAttack_Dash");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CHandAttack_Dash::Initialize()
{
    m_eAnimType = ANIM_ATTACK_HAND;
    m_iAnimIndex = 57;
    m_eStateType = STATE_HANDATTACK_DASH_PLAYER;

    m_vecAdjState.push_back(STATE_IDLE_PLAYER);
    m_vecAdjState.push_back(STATE_DASH_PLAYER);
    m_vecAdjState.push_back(STATE_NSATTACK_1_PLAYER);
    m_vecAdjState.push_back(STATE_HANDATTACK_2_PLAYER);
    m_vecAdjState.push_back(STATE_JUMP_PLAYER);
    m_vecAdjState.push_back(STATE_HANDATTACK_POWER_PLAYER);
    m_vecAdjState.push_back(STATE_RASENSURIKEN_PLAYER);
    m_vecAdjState.push_back(STATE_RASENGAN);
    m_vecAdjState.push_back(STATE_KAMUI_PLAYER);
    m_vecAdjState.push_back(STATE_EIGHTGATES);
    m_vecAdjState.push_back(STATE_CHIDORI);
    m_vecAdjState.push_back(STATE_FIREBALL_PLAYER);
    m_vecAdjState.push_back(STATE_THROW_PLAYER);

    m_fAnimSpeed = 0.9f;
    m_fInterPolationTime = 0.05f;
    m_fStateChangableTime = 0.25f;
    m_fDelayTime = 0.1f;

    /* For Attack */
    m_fSpeed = 8.f;
    m_fJumpPower = 0.f;
    m_fKnockbackPower = 8.f;
    m_eHitStateType = STATE_HIT_GROUND_ENEMY;
    m_iHitType = CHit_Ground::RIGHT;
    m_iColliderType = CUnit::LEFTHAND;

    return S_OK;
}

void CHandAttack_Dash::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    //CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_WIRE);
    

    CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_DEFAULT);

    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.5f;

    __super::Enter(pOwner, pAnimator);
}

STATE_TYPE CHandAttack_Dash::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CHandAttack_Dash::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    //CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_DEFAULT);
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;

    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CHandAttack_Dash::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    if (CUser::Get_Instance()->Get_LastKey() == KEY::LBUTTON)
    {
        return m_eStateType;
    }


    return STATE_END;
}

void CHandAttack_Dash::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
    //CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_QUICKDEFAULT);

    CFunctor::Play_Sound(L"Eff_Move", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
    CFunctor::Play_Sound(L"Voice_Man_Attack", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));
    __super::OnExecute(pOwner, pAnimator);
}
