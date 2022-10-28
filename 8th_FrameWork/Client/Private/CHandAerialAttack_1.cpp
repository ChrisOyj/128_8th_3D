#include "stdafx.h"
#include "CHandAerialAttack_1.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "CCamera_Free.h"
#include "Physics.h"

#include "CUser.h"

#include "CScript_FollowCam.h"

#include "CState_Manager.h"
#include "CHit_Air.h"

#include "CUnit_Player.h"

#include "Functor.h"

CHandAerialAttack_1::CHandAerialAttack_1()
{
}

CHandAerialAttack_1::~CHandAerialAttack_1()
{
}

CHandAerialAttack_1* CHandAerialAttack_1::Create()
{
    CHandAerialAttack_1* pInstance = new CHandAerialAttack_1();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CHandAerialAttack_1");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CHandAerialAttack_1::Initialize()
{
    m_eAnimType = ANIM_ATTACK_HAND;
    m_iAnimIndex = 6;
    m_eStateType = STATE_HANDAERIALATTACK_1_PLAYER;

    m_vecAdjState.push_back(STATE_FALL_PLAYER);
    m_vecAdjState.push_back(STATE_LAND_PLAYER);
    m_vecAdjState.push_back(STATE_HANDAERIALATTACK_2_PLAYER);
    m_vecAdjState.push_back(STATE_DOUBLEJUMP_PLAYER);
    m_vecAdjState.push_back(STATE_AIRDASHSTART_PLAYER);
    m_vecAdjState.push_back(STATE_NSAERIALATTACK_3_PLAYER);
    m_vecAdjState.push_back(STATE_HANDAERIALATTACK_4_PLAYER);
    m_vecAdjState.push_back(STATE_RASENSURIKEN_PLAYER);
    m_vecAdjState.push_back(STATE_CHIDORI);
    m_vecAdjState.push_back(STATE_FIREBALL_PLAYER);
    m_vecAdjState.push_back(STATE_THROW_PLAYER);
    m_vecAdjState.push_back(STATE_WATERPRISON_PLAYER);

    m_fAnimSpeed = 0.7f;
    m_fInterPolationTime = 0.1f;
    m_fStateChangableTime = 0.45f;
    m_fDelayTime = 0.28f;

    /* For Attack */
    m_fSpeed = 7.f;
    m_fColliderDisableTime = 0.1f;
    m_fJumpPower = 4.f;
    m_fKnockbackPower = 5.f;
    m_eHitStateType = STATE_HIT_AIR_ENEMY;
    m_iHitType = CHit_Air::SHORT;
    m_iColliderType = CUnit::RIGHTHAND;

    return S_OK;
}

void CHandAerialAttack_1::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_DEFAULT);

    __super::Enter(pOwner, pAnimator);
    pOwner->Get_PhysicsCom()->Set_Jump(0.f);
    pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 0.f;
    pOwner->Get_PhysicsCom()->Mul_Speed(0.5f);


}

STATE_TYPE CHandAerialAttack_1::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    /*if (m_fTimeAcc >= m_fStateChangableTime)
    {
        if (pOwner->Get_PhysicsCom()->Get_Physics().fGravity < 9.8f)
        {
            pOwner->Get_PhysicsCom()->Set_Jump(0.f);
            pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 9.8f;
        }
    }*/

    return __super::Tick(pOwner, pAnimator);
}

void CHandAerialAttack_1::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);

    if (pOwner->Get_PhysicsCom()->Get_Physics().fGravity < 9.8f)
    {
        pOwner->Get_PhysicsCom()->Set_Jump(0.f);
        pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 9.8f;
    }
    
}

STATE_TYPE CHandAerialAttack_1::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    if (CUser::Get_Instance()->Get_LastKey() == KEY::LBUTTON
        && (pOwner->Is_Hand() || pOwner->Is_Water()))
    {
        return m_eStateType;
    }


    return STATE_END;
}

void CHandAerialAttack_1::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
    CFunctor::Play_Sound(L"Eff_Move", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
    CFunctor::Play_Sound(L"Voice_Man_Attack", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));
    __super::OnExecute(pOwner, pAnimator);
}
