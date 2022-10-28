#include "stdafx.h"
#include "CHandAttack_2_Enemy.h"

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

CHandAttack_2_Enemy::CHandAttack_2_Enemy()
{
}

CHandAttack_2_Enemy::~CHandAttack_2_Enemy()
{
}

CHandAttack_2_Enemy* CHandAttack_2_Enemy::Create()
{
    CHandAttack_2_Enemy* pInstance = new CHandAttack_2_Enemy();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CHandAttack_2_Enemy");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CHandAttack_2_Enemy::Initialize()
{
    m_eAnimType = ANIM_ATTACK_HAND;
    m_iAnimIndex = 60;
    m_eStateType = STATE_HANDATTACK_2_ENEMY;

    //멀어졌으면 run
    m_vecAdjState.push_back(STATE_RUN_ENEMY);
    m_vecAdjState.push_back(STATE_JUMP_ENEMY);

    //공격범위 안이면 공격, 대쉬
    m_vecAdjState.push_back(STATE_DASH_ENEMY);
    m_vecAdjState.push_back(STATE_HANDATTACK_3_ENEMY);

    m_fAnimSpeed = 2.5f;
    m_fInterPolationTime = 0.05f;
    m_fStateChangableTime = 0.45f;
    m_fDelayTime = 0.2f;

    /* For Attack */
    m_fSpeed = 8.f;
    m_fJumpPower = 0.f;
    m_fKnockbackPower = 11.f;
    m_eHitStateType = STATE_HIT_GROUND_PLAYER;
    m_iHitType = CHit_Ground::LEFT;
    m_iColliderType = CUnit::RIGHTHAND;

    return S_OK;
}

void CHandAttack_2_Enemy::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Enter(pOwner, pAnimator);

}

STATE_TYPE CHandAttack_2_Enemy::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);
}

void CHandAttack_2_Enemy::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CHandAttack_2_Enemy::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    _uint iRand = random(0, 1);
    if (1)
    {
        return m_eStateType;
    }


    return STATE_END;
}

void CHandAttack_2_Enemy::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::OnExecute(pOwner, pAnimator);
    CFunctor::Play_Sound(L"Eff_Move", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

}
