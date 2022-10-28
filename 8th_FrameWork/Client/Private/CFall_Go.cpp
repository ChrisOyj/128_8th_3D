#include "stdafx.h"
#include "CFall_Go.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "CCamera_Free.h"
#include "Physics.h"

CFall_Go::CFall_Go()
{
}

CFall_Go::~CFall_Go()
{
}

CFall_Go* CFall_Go::Create()
{
    CFall_Go* pInstance = new CFall_Go();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CFall_Go");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CFall_Go::Initialize()
{
    m_iAnimIndex = 30;
    m_eStateType = STATE_FALL_GO;

    m_vecAdjState.push_back(STATE_LAND_GO);

    m_fInterPolationTime = 0.2f;
    m_fStateChangableTime = 0.f;
    return S_OK;
}

void CFall_Go::Enter(CUnit* pOwner, CAnimator* pAnimator)
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

STATE_TYPE CFall_Go::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);
}

void CFall_Go::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CFall_Go::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Run으로 오는 조건
    1. 키눌렸을때
    */
    if (pAnimator->Is_CurAnimFinished() && pOwner->Get_PhysicsCom()->Get_Physics().bAir)
        return m_eStateType;


    return STATE_END;
}
