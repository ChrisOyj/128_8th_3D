#include "stdafx.h"
#include "CHandAttack_4_Enemy.h"

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
#include "CBlasted_Ground.h"
#include "Functor.h"

CHandAttack_4_Enemy::CHandAttack_4_Enemy()
{
}

CHandAttack_4_Enemy::~CHandAttack_4_Enemy()
{
}

CHandAttack_4_Enemy* CHandAttack_4_Enemy::Create()
{
    CHandAttack_4_Enemy* pInstance = new CHandAttack_4_Enemy();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CHandAttack_4_Enemy");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

void CHandAttack_4_Enemy::OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos)
{
}

HRESULT CHandAttack_4_Enemy::Initialize()
{
    m_eAnimType = ANIM_ATTACK_HAND;
    m_iAnimIndex = 62;
    m_eStateType = STATE_HANDATTACK_4_ENEMY;

    //멀어졌으면 run
    m_vecAdjState.push_back(STATE_RUN_ENEMY);
    m_vecAdjState.push_back(STATE_JUMP_ENEMY);

    //공격범위 안이면 공격, 대쉬
    m_vecAdjState.push_back(STATE_DASH_ENEMY);
    m_vecAdjState.push_back(STATE_GUARD_ENEMY);
    m_vecAdjState.push_back(STATE_HANDATTACK_1_ENEMY);


    m_fInterPolationTime = 0.1f;
    m_fStateChangableTime = 0.7f;
    m_fDelayTime = 0.35f;

    /* For Attack */
    m_fSpeed = 11.f;
    m_fJumpPower = 5.f;
    m_fKnockbackPower = 12.f;
    m_eHitStateType = STATE_BLASTED_GROUND_PLAYER;
    m_iHitType = CHit_Ground::HEAD;
    m_iColliderType = CUnit::LEFTFOOT;

    return S_OK;
}

void CHandAttack_4_Enemy::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Enter(pOwner, pAnimator);

}

STATE_TYPE CHandAttack_4_Enemy::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CHandAttack_4_Enemy::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);

}

STATE_TYPE CHandAttack_4_Enemy::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    if (1)
    {
        return m_eStateType;
    }


    return STATE_END;
}

void CHandAttack_4_Enemy::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::OnExecute(pOwner, pAnimator);
    CFunctor::Play_Sound(L"Eff_Move", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

}
