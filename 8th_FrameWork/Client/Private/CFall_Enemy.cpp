#include "stdafx.h"
#include "CFall_Enemy.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "CCamera_Free.h"
#include "Physics.h"

CFall_Enemy::CFall_Enemy()
{
}

CFall_Enemy::~CFall_Enemy()
{
}

CFall_Enemy* CFall_Enemy::Create()
{
    CFall_Enemy* pInstance = new CFall_Enemy();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CFall_Enemy");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CFall_Enemy::Initialize()
{
    m_iAnimIndex = 30;
    m_eStateType = STATE_FALL_ENEMY;

    m_vecAdjState.push_back(STATE_LAND_ENEMY);

    m_fInterPolationTime = 0.2f;
    m_fStateChangableTime = 0.f;
    return S_OK;
}

void CFall_Enemy::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pOwner->Get_PhysicsCom()->Get_Physics().fSpeed <= 1.f)
    {
        m_iAnimIndex = 31;
    }
    else
    {
        m_iAnimIndex = 30;
    }

    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator);

}

STATE_TYPE CFall_Enemy::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);
}

void CFall_Enemy::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CFall_Enemy::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Run으로 오는 조건
    1. 키눌렸을때
    */
    if (pAnimator->Is_CurAnimFinished())
        return m_eStateType;


    return STATE_END;
}
