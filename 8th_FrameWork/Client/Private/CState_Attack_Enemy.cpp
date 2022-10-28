#include "stdafx.h"
#include "CState_Attack_Enemy.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "CCamera_Free.h"
#include "Physics.h"

#include "CUser.h"

#include "CScript_FollowCam.h"

#include "CState_Manager.h"
#include "CBlasted_Ground_Player.h"
#include "CHit_Air_Player.h"
#include "CHit_Ground_Player.h"

#include "CUnit_Player.h"

CState_Attack_Enemy::CState_Attack_Enemy()
{
}

CState_Attack_Enemy::~CState_Attack_Enemy()
{
}

void CState_Attack_Enemy::OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos)
{
}

HRESULT CState_Attack_Enemy::Initialize()
{
    m_fTargetFindMaxDistance = 4.f;
    m_fColliderDisableTime = 0.2f;
    return S_OK;
}

void CState_Attack_Enemy::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->CallBack_CollisionEnter += bind(&CState_Attack_Enemy::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3);

    m_fAttackTimeAcc = 0.f;
    m_bDisableCollider = false;

    CTransform* pTransform = pOwner->Get_Transform();
    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();

    CUnit* pTargetUnit = pOwner->Get_TargetUnit();

    _float4 vFinalLook;
    _bool bTarget = false;

    if (pTargetUnit)
    {
        _float4 vMyPos = pTransform->Get_World(WORLD_POS);
        _float4 vTargetPos = pTargetUnit->Get_Transform()->Get_World(WORLD_POS);

        _float fLength = (vMyPos - vTargetPos).Length();

        if (fLength < m_fTargetFindMaxDistance)
        {
            bTarget = true;
        }
    }

    if (bTarget)
    {
        _float4 vMyPos = pTransform->Get_World(WORLD_POS);
        _float4 vTargetPos = pTargetUnit->Get_Transform()->Get_World(WORLD_POS);
        vFinalLook = (vTargetPos - vMyPos);

        vFinalLook.y = 0.f;
        vFinalLook.Normalize();
        pTransform->Set_LerpLook(vFinalLook, m_fDelayTime);

    }




    __super::Enter(pOwner, pAnimator);

}

STATE_TYPE CState_Attack_Enemy::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    //타겟이 있으면 타겟쪽으로 가야함
    if (m_fTimeAcc <= m_fDelayTime) //공격실행 전까지만
    {
        CUnit* pTargetUnit = pOwner->Get_TargetUnit();
        if (pTargetUnit)
        {
            _float4 vMyPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
            _float4 vTargetPos = pTargetUnit->Get_Transform()->Get_World(WORLD_POS);

            _float4 vDir = (vTargetPos - vMyPos);

            if (vDir.Length() < m_fTargetFindMaxDistance)
            {
                vDir.y = 0.f;
                _float4 vGoalPos = vTargetPos + vDir.Normalize() * -0.3f;

                vDir = (vGoalPos - vMyPos);
                vMyPos += vDir * 4.f * fDT;

                pOwner->Get_Transform()->Set_World(WORLD_POS, vMyPos);
                pOwner->Get_Transform()->Make_WorldMatrix();
            }
        }

    }

    return __super::Tick(pOwner, pAnimator);
}

void CState_Attack_Enemy::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    m_bStopTime = false;
    m_fStopTimeAcc = 0.f;
    pOwner->CallBack_CollisionEnter -= bind(&CState_Attack_Enemy::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3);
    pOwner->Enable_Collider(CUnit::COLLIDER_PARTS(m_iColliderType), false);
}

STATE_TYPE CState_Attack_Enemy::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

void CState_Attack_Enemy::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
    _float4 vLook = pOwner->Get_Transform()->Get_World(WORLD_LOOK);
    vLook.y = 0.f;
    vLook.Normalize();

    pOwner->Enable_Collider(CUnit::COLLIDER_PARTS(m_iColliderType), true);
    pOwner->Get_PhysicsCom()->Set_Dir(vLook);
    pOwner->Get_PhysicsCom()->Set_MaxSpeed(m_fSpeed);
    pOwner->Get_PhysicsCom()->Set_Speed(m_fSpeed);

    switch (m_eHitStateType)
    {
    case STATE_HIT_GROUND_PLAYER:
    {
        static_cast<CHit_Ground_Player*>(CState_Manager::Get_Instance()->Get_State(STATE_HIT_GROUND_PLAYER))->On_HitGround(
            CHit_Ground_Player::HIT_TYPE(m_iHitType), vLook * -1.f, vLook, m_fKnockbackPower, m_fJumpPower, m_fDamage);
    }
    break;

    case STATE_BLASTED_GROUND_PLAYER:
    {
        static_cast<CBlasted_Ground_Player*>(CState_Manager::Get_Instance()->Get_State(STATE_BLASTED_GROUND_PLAYER))->On_BlastedGround(
            vLook * -1.f, vLook, m_fKnockbackPower, m_fJumpPower, m_fDamage);
    }
    break;

    case STATE_HIT_AIR_PLAYER:
    {
        static_cast<CHit_Air_Player*>(CState_Manager::Get_Instance()->Get_State(STATE_HIT_AIR_PLAYER))->On_HitAir(
            CHit_Air_Player::HIT_TYPE(m_iHitType), vLook * -1.f, vLook, m_fKnockbackPower, m_fJumpPower, m_fDamage);
    }
    break;

    default:
        break;
    }


}

void CState_Attack_Enemy::TickExecute(CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_bDisableCollider)
        return;

    m_fAttackTimeAcc += fDT;

    if (m_fAttackTimeAcc >= m_fColliderDisableTime)
        OnDisableCollider(pOwner, pAnimator);
}

void CState_Attack_Enemy::OnDisableCollider(CUnit* pOwner, CAnimator* pAnimator)
{
    m_bDisableCollider = true;
    pOwner->Enable_Collider(CUnit::COLLIDER_PARTS(m_iColliderType), false);

}
