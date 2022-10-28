#include "stdafx.h"
#include "CDash_Enemy.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "CCamera_Free.h"
#include "Physics.h"

#include "CUser.h"

#include "CScript_FollowCam.h"
#include "CEffects_Factory.h"
#include "CCell.h"
#include "CNavigation.h"
#include "Functor.h"
CDash_Enemy::CDash_Enemy()
{
}

CDash_Enemy::~CDash_Enemy()
{
}

CDash_Enemy* CDash_Enemy::Create()
{
    CDash_Enemy* pInstance = new CDash_Enemy();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CDash_Enemy");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CDash_Enemy::Initialize()
{
    m_iAnimIndex = 24;
    m_eStateType = STATE_DASH_ENEMY;

    m_vecAdjState.push_back(STATE_CHIDORI_ENEMY);

    m_vecAdjState.push_back(STATE_RUN_ENEMY);
    m_vecAdjState.push_back(STATE_JUMP_ENEMY);
    m_vecAdjState.push_back(STATE_THROW_ENEMY);
    m_vecAdjState.push_back(STATE_GUARD_ENEMY);
    m_vecAdjState.push_back(STATE_IDLE_ENEMY);
    m_vecAdjState.push_back(STATE_HANDATTACK_1_ENEMY);


    m_fInterPolationTime = 0.1f;
    m_fStateChangableTime = 0.7f;
    m_fAnimSpeed = 1.1f;
    m_fDelayTime = 0.4f;

    return S_OK;
}

void CDash_Enemy::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    CFunctor::Play_Sound(L"Eff_GroundDash", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

    CEffects_Factory::Get_Instance()->Create_LandingEffects(pOwner->Get_Transform()->Get_World(WORLD_POS), GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());

    CTransform* pTransform = pOwner->Get_Transform();
    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();
    pOwner->Disable_AllColliders();

    m_iAnimIndex = 24;
    m_fStateChangableTime = 0.75f;

    _float fDashSpeed = pOwner->Get_Status().fDashSpeed;
    _float4 vCurPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
    _float4 vTargetPos = pOwner->Get_TargetUnit()->Get_Transform()->Get_World(WORLD_POS);
    vTargetPos.y = 0.f;
    vCurPos.y = 0.f;
    _float4 vDir = vTargetPos - vCurPos;
    _float4 vRight = pOwner->Get_Transform()->Get_World(WORLD_RIGHT);
    _float fTargetLength = vDir.Length();
    vDir.Normalize();
    _int iRand = random(0, 3);

    _float4 vFinalDir;
    switch (iRand)
    {
    case 0:
        vFinalDir = vDir;
        m_iAnimIndex = 24;
        break;
    case 1://left
        vFinalDir = vRight * -1.f;
        m_iAnimIndex = 25;
        break;
    case 2://right
        vFinalDir = vRight;
        m_iAnimIndex = 26;
        break;
    case 3:
        vFinalDir = vDir * -1.f;
        m_iAnimIndex = 23;
        break;

    default:
        break;
    }


    pPhyscisCom->Set_MaxSpeed(fDashSpeed);
    pPhyscisCom->Set_Speed(fDashSpeed);
    _float4 vTargetLook = vDir.Normalize();

    pTransform->Set_LerpLook(vDir, 0.4f);

    if (m_iAnimIndex == 24)
    {
        pTransform->Set_Look(vDir);
        pTransform->Make_WorldMatrix();
        CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Dash_Effects_Ground", pOwner, pTransform->Get_World(WORLD_POS));
    }

    pPhyscisCom->Set_Dir(vFinalDir);
    pPhyscisCom->Get_PhysicsDetail().fFrictionRatio = 0.5f;

    __super::Enter(pOwner, pAnimator);

}

STATE_TYPE CDash_Enemy::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (0.4f <= m_fTimeAcc + fDT &&
        pOwner->Get_PhysicsCom()->Get_Physics().bAir)
    {
        return STATE_FALL_ENEMY;
    }

    return __super::Tick(pOwner, pAnimator);
}

void CDash_Enemy::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;

    if (pOwner->Get_PhysicsCom()->Get_Physics().bAir)
        pOwner->Get_PhysicsCom()->Set_Speed(pOwner->Get_PhysicsCom()->Get_Physics().fSpeed * 0.5f);

    pOwner->Enable_BodyColliders();

}

STATE_TYPE CDash_Enemy::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Enemy가 Dash으로 오는 조건
    1. 
    어느정도 가까움

    */
    _float4 vCurPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
    _float4 vTargetPos = pOwner->Get_TargetUnit()->Get_Transform()->Get_World(WORLD_POS);
    vTargetPos.y = 0.f;
    vCurPos.y = 0.f;
    _float4 vDir = vTargetPos - vCurPos;
    _float fTargetLength = vDir.Length();


    if (fTargetLength < 4.f)
    {
        _uint iRand = random(0, 2);

        if (iRand == 0)
            return m_eStateType;
    }


    return STATE_END;
}

void CDash_Enemy::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->Enable_BodyColliders();
}
