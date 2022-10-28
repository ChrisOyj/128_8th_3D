#include "stdafx.h"
#include "CHandAttack_1_Enemy.h"

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

#include "CUnit_Player.h"

#include "CEffects_Factory.h"
#include "CCell.h"
#include "CNavigation.h"
#include "Functor.h"
CHandAttack_1_Enemy::CHandAttack_1_Enemy()
{
}

CHandAttack_1_Enemy::~CHandAttack_1_Enemy()
{
}

CHandAttack_1_Enemy* CHandAttack_1_Enemy::Create()
{
    CHandAttack_1_Enemy* pInstance = new CHandAttack_1_Enemy();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CHandAttack_1_Enemy");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

void CHandAttack_1_Enemy::OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos)
{
}

HRESULT CHandAttack_1_Enemy::Initialize()
{
    m_eAnimType = ANIM_ATTACK_HAND;
    m_iAnimIndex = 40;
    m_eStateType = STATE_HANDATTACK_1_ENEMY;

    //멀어졌으면 run
    m_vecAdjState.push_back(STATE_RUN_ENEMY);
    m_vecAdjState.push_back(STATE_JUMP_ENEMY);

    //공격범위 안이면 공격, 대쉬
    m_vecAdjState.push_back(STATE_DASH_ENEMY);
    m_vecAdjState.push_back(STATE_HANDATTACK_2_ENEMY);

    m_fAnimSpeed = 1.4f;
    m_fInterPolationTime = 0.05f;
    m_fStateChangableTime = 0.45f;
    m_fDelayTime = 0.3f;

    /* For Attack */
    m_fSpeed = 7.f;
    m_fJumpPower = 0.f;
    m_fKnockbackPower = 5.f;
    m_eHitStateType = STATE_HIT_GROUND_PLAYER;
    m_iHitType = CHit_Ground::RIGHT;
    m_iColliderType = CUnit::LEFTHAND;

    return S_OK;
}

void CHandAttack_1_Enemy::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    CEffects_Factory::Get_Instance()->Create_LandingEffects(pOwner->Get_Transform()->Get_World(WORLD_POS), GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());

    __super::Enter(pOwner, pAnimator);
}

STATE_TYPE CHandAttack_1_Enemy::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CHandAttack_1_Enemy::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CHandAttack_1_Enemy::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    _float4 vCurPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
    _float4 vTargetPos = pOwner->Get_TargetUnit()->Get_Transform()->Get_World(WORLD_POS);
    vTargetPos.y = 0.f;
    vCurPos.y = 0.f;
    _float4 vDir = vTargetPos - vCurPos;
    _float fTargetLength = vDir.Length();


    if (fTargetLength < 4.f)
    {
            return m_eStateType;
    }


    return STATE_END;
}

void CHandAttack_1_Enemy::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
    CFunctor::Play_Sound(L"Eff_Move", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
    __super::OnExecute(pOwner, pAnimator);
}
