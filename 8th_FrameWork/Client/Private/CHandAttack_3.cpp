#include "stdafx.h"
#include "CHandAttack_3.h"

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
#include "CHit_Ground.h"

#include "Functor.h"

CHandAttack_3::CHandAttack_3()
{
}

CHandAttack_3::~CHandAttack_3()
{
}

CHandAttack_3* CHandAttack_3::Create()
{
    CHandAttack_3* pInstance = new CHandAttack_3();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CHandAttack_3");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CHandAttack_3::Initialize()
{
    m_eAnimType = ANIM_ATTACK_HAND;
    m_iAnimIndex = 55;
    m_eStateType = STATE_HANDATTACK_3_PLAYER;
    m_fAnimSpeed = 1.9f;

    m_vecAdjState.push_back(STATE_IDLE_PLAYER);
    m_vecAdjState.push_back(STATE_DASH_PLAYER);
    m_vecAdjState.push_back(STATE_NSATTACK_4_PLAYER);
    m_vecAdjState.push_back(STATE_HANDATTACK_4_PLAYER);
    m_vecAdjState.push_back(STATE_JUMP_PLAYER);
    m_vecAdjState.push_back(STATE_HANDATTACK_POWER_PLAYER);
    m_vecAdjState.push_back(STATE_RASENSURIKEN_PLAYER);
    m_vecAdjState.push_back(STATE_RASENGAN);
    m_vecAdjState.push_back(STATE_KAMUI_PLAYER);
    m_vecAdjState.push_back(STATE_EIGHTGATES);
    m_vecAdjState.push_back(STATE_CHIDORI);
    m_vecAdjState.push_back(STATE_FIREBALL_PLAYER);
    m_vecAdjState.push_back(STATE_THROW_PLAYER);

    m_fInterPolationTime = 0.0f;
    m_fStateChangableTime = 0.5f;
    m_fDelayTime = 0.25f;

    /* For Attack */
    m_fSpeed = 14.f;
    m_fJumpPower = 5.f;
    m_fColliderDisableTime += 0.1f;
    m_fKnockbackPower = 6.f;
    m_eHitStateType = STATE_HIT_GROUND_ENEMY;
    m_iHitType = CHit_Ground::HEAD;
    m_iColliderType = CUnit::RIGHTFOOT;

    return S_OK;
}

void CHandAttack_3::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Enter(pOwner, pAnimator);
}

STATE_TYPE CHandAttack_3::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);
}

void CHandAttack_3::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->Get_PhysicsCom()->Mul_Speed(0.5f);
    static_cast<CUnit_Player*>(pOwner)->TurnOff_TrailBuffer();
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CHandAttack_3::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    if (CUser::Get_Instance()->Get_LastKey() == KEY::LBUTTON)
    {
        return m_eStateType;
    }


    return STATE_END;
}

void CHandAttack_3::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::OnExecute(pOwner, pAnimator);
    //static_cast<CUnit_Player*>(pOwner)->TurnOn_TrailBuffer();

    pOwner->Get_PhysicsCom()->Set_Jump(2.f);

    CFunctor::Play_Sound(L"Eff_Move", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
    CFunctor::Play_Sound(L"Voice_Man_Attack", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));
}

void CHandAttack_3::OnDisableCollider(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::OnDisableCollider(pOwner, pAnimator);
    //static_cast<CUnit_Player*>(pOwner)->TurnOff_TrailBuffer();

    pOwner->Get_PhysicsCom()->Mul_Speed(0.5f);

}
