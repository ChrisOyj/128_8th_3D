#include "stdafx.h"
#include "CState_Attack.h"

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
#include "CHit_Air.h"

#include "CUnit_Player.h"

#include "CEffects_Factory.h"
#include "CCell.h"
#include "CNavigation.h"
#include "Functor.h"
CState_Attack::CState_Attack()
{
}

CState_Attack::~CState_Attack()
{
}
void CState_Attack::OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos)
{
    if (eColType == COL_ENEMYHITBOX)
    {
      CFunctor::Play_Sound(L"Eff_Hit_Hand", CHANNEL_EFFECTS, PLAYER->Get_Transform()->Get_World(WORLD_POS));
    }
        
    
}

HRESULT CState_Attack::Initialize()
{
    m_fTargetFindMaxDistance = 4.f;
    m_fColliderDisableTime = 0.2f;
    return S_OK;
}

void CState_Attack::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->CallBack_CollisionEnter += bind(&CState_Attack::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3);

    m_fAttackTimeAcc = 0.f;
    m_bDisableCollider = false;

    CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_DEFAULT);

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
    else
    {
        /*pTargetUnit = CUser::Get_Instance()->Find_RealClosestEnemy();

        if (pTargetUnit)
        {
            bTarget = true;
            PLAYER->Set_TargetUnit(pTargetUnit);
            CUser::Get_Instance()->Set_Target(pTargetUnit);
        }*/
    }



    if (bTarget)
    {
        _float4 vMyPos = pTransform->Get_World(WORLD_POS);
        _float4 vTargetPos = pTargetUnit->Get_Transform()->Get_World(WORLD_POS);
        vFinalLook = (vTargetPos - vMyPos);

           vFinalLook.y = 0.f;
        vFinalLook.Normalize();
    }
    else
    {
        _float4 vUserLook = CGameInstance::Get_Instance()->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
        vUserLook.Normalize();

        _float4 _vLook = vUserLook;
        _float4 vUp = pTransform->Get_World(WORLD_UP);
        _float4 vRight = vUp.Cross(_vLook);
        _vLook = vRight.Cross(vUp.Normalize());
        _vLook.Normalize();

        _float4 vDir = { 0.f, 0.f, 0.f, 0.f };
        if (KEY(W, HOLD))
        {
            vDir += _vLook;
        }
        if (KEY(A, HOLD))
        {
            vDir -= vRight;
        }
        if (KEY(D, HOLD))
        {
            vDir += vRight;
        }

        if (KEY(S, HOLD))
        {
            vDir -= _vLook;
        }

        if (vDir.Is_Zero())
        {
            __super::Enter(pOwner, pAnimator);
            return;
        }

        _float4 vTargetLook = vDir.Normalize();

        vFinalLook = vTargetLook;
    }


    pTransform->Set_LerpLook(vFinalLook, m_fDelayTime);


    __super::Enter(pOwner, pAnimator);

}

STATE_TYPE CState_Attack::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    /*if (m_bStopTime)
    {
        m_fStopTimeAcc += RealfDT;
        if (m_fStopTimeAcc >= 0.1f)
        {
            m_bStopTime = false;
            m_fStopTimeAcc = 0.f;
            GAMEINSTANCE->Set_TimeSpeed(1.f);

        }
    }*/

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

void CState_Attack::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    m_bStopTime = false;
    m_fStopTimeAcc = 0.f;
    pOwner->CallBack_CollisionEnter -= bind(&CState_Attack::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3);
    pOwner->Enable_Collider(CUnit::COLLIDER_PARTS(m_iColliderType), false);
}

STATE_TYPE CState_Attack::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

void CState_Attack::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
    if (!pOwner->Get_PhysicsCom()->Get_Physics().bAir)
        CEffects_Factory::Get_Instance()->Create_LandingEffects(pOwner->Get_Transform()->Get_World(WORLD_POS), GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());

    _float4 vLook = pOwner->Get_Transform()->Get_World(WORLD_LOOK);
    vLook.y = 0.f;
    vLook.Normalize();

    pOwner->Enable_Collider(CUnit::COLLIDER_PARTS(m_iColliderType), true);
    pOwner->Get_PhysicsCom()->Set_Dir(vLook);
    pOwner->Get_PhysicsCom()->Set_MaxSpeed(m_fSpeed);
    pOwner->Get_PhysicsCom()->Set_Speed(m_fSpeed);

    switch (m_eHitStateType)
    {
    case STATE_HIT_GROUND_ENEMY:
    {
        static_cast<CHit_Ground*>(CState_Manager::Get_Instance()->Get_State(STATE_HIT_GROUND_ENEMY))->On_HitGround(
            CHit_Ground::HIT_TYPE(m_iHitType), vLook * -1.f, vLook, m_fKnockbackPower, m_fJumpPower, m_fDamage);
    }
    break;

    case STATE_BLASTED_GROUND_ENEMY:
    {
        static_cast<CBlasted_Ground*>(CState_Manager::Get_Instance()->Get_State(STATE_BLASTED_GROUND_ENEMY))->On_BlastedGround(
            vLook * -1.f, vLook, m_fKnockbackPower, m_fJumpPower, m_fDamage);
    }
    break;

    case STATE_HIT_AIR_ENEMY:
    {
        static_cast<CHit_Air*>(CState_Manager::Get_Instance()->Get_State(STATE_HIT_AIR_ENEMY))->On_HitAir(
            CHit_Air::HIT_TYPE(m_iHitType), vLook * -1.f, vLook, m_fKnockbackPower, m_fJumpPower, m_fDamage);
    }
    break;

    default:
        break;
    }
   

}

void CState_Attack::TickExecute(CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_bDisableCollider)
        return;

    m_fAttackTimeAcc += fDT;

    if (m_fAttackTimeAcc >= m_fColliderDisableTime)
        OnDisableCollider(pOwner, pAnimator);
}

void CState_Attack::OnDisableCollider(CUnit* pOwner, CAnimator* pAnimator)
{
    m_bDisableCollider = true;
    pOwner->Enable_Collider(CUnit::COLLIDER_PARTS(m_iColliderType), false);

}
