#include "stdafx.h"
#include "CHandAttack_2.h"

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

CHandAttack_2::CHandAttack_2()
{
}

CHandAttack_2::~CHandAttack_2()
{
}

CHandAttack_2* CHandAttack_2::Create()
{
    CHandAttack_2* pInstance = new CHandAttack_2();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CHandAttack_2");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CHandAttack_2::Initialize()
{
    m_eAnimType = ANIM_ATTACK_HAND;
    m_iAnimIndex = 60;
    m_eStateType = STATE_HANDATTACK_2_PLAYER;

    m_vecAdjState.push_back(STATE_IDLE_PLAYER);
    m_vecAdjState.push_back(STATE_DASH_PLAYER);
    m_vecAdjState.push_back(STATE_NSATTACK_3_PLAYER);
    m_vecAdjState.push_back(STATE_HANDATTACK_3_PLAYER);
    m_vecAdjState.push_back(STATE_JUMP_PLAYER);
    m_vecAdjState.push_back(STATE_HANDATTACK_POWER_PLAYER);
    m_vecAdjState.push_back(STATE_RASENSURIKEN_PLAYER);
    m_vecAdjState.push_back(STATE_RASENGAN);
    m_vecAdjState.push_back(STATE_KAMUI_PLAYER);
    m_vecAdjState.push_back(STATE_EIGHTGATES);
    m_vecAdjState.push_back(STATE_CHIDORI);
    m_vecAdjState.push_back(STATE_FIREBALL_PLAYER);
    m_vecAdjState.push_back(STATE_THROW_PLAYER);

    m_fAnimSpeed = 2.5f;
    m_fInterPolationTime = 0.05f;
    m_fStateChangableTime = 0.35f;
    m_fDelayTime = 0.2f;

    /* For Attack */
    m_fSpeed = 8.f;
    m_fJumpPower = 0.f;
    m_fKnockbackPower = 11.f;
    m_eHitStateType = STATE_HIT_GROUND_ENEMY;
    m_iHitType = CHit_Ground::LEFT;
    m_iColliderType = CUnit::RIGHTHAND;

    return S_OK;
}

void CHandAttack_2::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Enter(pOwner, pAnimator);

}

STATE_TYPE CHandAttack_2::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);
}

void CHandAttack_2::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CHandAttack_2::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    if (CUser::Get_Instance()->Get_LastKey() == KEY::LBUTTON)
    {
        return m_eStateType;
    }


    return STATE_END;
}

void CHandAttack_2::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::OnExecute(pOwner, pAnimator);

    CFunctor::Play_Sound(L"Eff_Move", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
    CFunctor::Play_Sound(L"Voice_Man_Attack", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));
}
