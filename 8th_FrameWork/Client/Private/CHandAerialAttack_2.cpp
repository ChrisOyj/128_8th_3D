#include "stdafx.h"
#include "CHandAerialAttack_2.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "CCamera_Free.h"
#include "Physics.h"

#include "CUser.h"

#include "CScript_FollowCam.h"

#include "CState_Manager.h"
#include "CUnit_Player.h"
#include "CHit_Air.h"

#include "Functor.h"

CHandAerialAttack_2::CHandAerialAttack_2()
{
}

CHandAerialAttack_2::~CHandAerialAttack_2()
{
}

CHandAerialAttack_2* CHandAerialAttack_2::Create()
{
    CHandAerialAttack_2* pInstance = new CHandAerialAttack_2();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CHandAerialAttack_2");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CHandAerialAttack_2::Initialize()
{
    m_eAnimType = ANIM_ATTACK_HAND;
    m_iAnimIndex = 7;
    m_eStateType = STATE_HANDAERIALATTACK_2_PLAYER;

    m_vecAdjState.push_back(STATE_FALL_PLAYER);
    m_vecAdjState.push_back(STATE_LAND_PLAYER);
    m_vecAdjState.push_back(STATE_HANDAERIALATTACK_3_PLAYER);
    m_vecAdjState.push_back(STATE_DOUBLEJUMP_PLAYER);
    m_vecAdjState.push_back(STATE_AIRDASHSTART_PLAYER);
    m_vecAdjState.push_back(STATE_NSAERIALATTACK_3_PLAYER);
    m_vecAdjState.push_back(STATE_HANDAERIALATTACK_4_PLAYER);
    m_vecAdjState.push_back(STATE_RASENSURIKEN_PLAYER);
    m_vecAdjState.push_back(STATE_CHIDORI);
    m_vecAdjState.push_back(STATE_FIREBALL_PLAYER);
    m_vecAdjState.push_back(STATE_THROW_PLAYER);
    m_vecAdjState.push_back(STATE_WATERPRISON_PLAYER);

    m_fAnimSpeed = 1.f;
    m_fInterPolationTime = 0.15f;
    m_fStateChangableTime = 0.5f;
    m_fDelayTime = 0.3f;

    /* For Attack */
    m_fSpeed = 7.f;
    m_fColliderDisableTime = 0.1f;
    m_fJumpPower = 5.f;
    m_fKnockbackPower = 5.f;
    m_eHitStateType = STATE_HIT_AIR_ENEMY;
    m_iHitType = CHit_Air::SHORT;
    m_iColliderType = CUnit::RIGHTFOOT;

    return S_OK;
}

void CHandAerialAttack_2::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_DEFAULT);

    __super::Enter(pOwner, pAnimator);
    pOwner->Get_PhysicsCom()->Set_Jump(0.f);
    pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 0.f;
    pOwner->Get_PhysicsCom()->Mul_Speed(0.5f);
}

STATE_TYPE CHandAerialAttack_2::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
   /* if (m_fTimeAcc >= m_fStateChangableTime)
    {
        if (pOwner->Get_PhysicsCom()->Get_Physics().fGravity < 9.8f)
        {
            pOwner->Get_PhysicsCom()->Set_Jump(0.f);
            pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 9.8f;
        }
    }*/

    return __super::Tick(pOwner, pAnimator);
}

void CHandAerialAttack_2::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);

    if (pOwner->Get_PhysicsCom()->Get_Physics().fGravity < 9.8f)
    {
        pOwner->Get_PhysicsCom()->Set_Jump(0.f);
        pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 9.8f;
    }

    _float4 vLook = pOwner->Get_Transform()->Get_World(WORLD_LOOK);
    vLook.y = 0.f;
    vLook.Normalize();
    pOwner->Get_Transform()->Set_LerpLook(vLook, 0.3f);
}

STATE_TYPE CHandAerialAttack_2::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    if (CUser::Get_Instance()->Get_LastKey() == KEY::LBUTTON
        && (pOwner->Is_Hand() || pOwner->Is_Water()))
    {
        return m_eStateType;
    }


    return STATE_END;
}

void CHandAerialAttack_2::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
    CFunctor::Play_Sound(L"Eff_Move", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
    CFunctor::Play_Sound(L"Voice_Man_Attack", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));
    __super::OnExecute(pOwner, pAnimator);
}
