#include "stdafx.h"
#include "CGetUp_Enemy.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "CCamera_Free.h"
#include "Physics.h"

#include "CState_Manager.h"
#include "CDoubleJump_Player.h"

#include "CNavigation.h"

#include "CUser.h"
#include "CScript_FollowCam.h"

CGetUp_Enemy::CGetUp_Enemy()
{
}

CGetUp_Enemy::~CGetUp_Enemy()
{
}

CGetUp_Enemy* CGetUp_Enemy::Create()
{
    CGetUp_Enemy* pInstance = new CGetUp_Enemy();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CGetUp_Enemy");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CGetUp_Enemy::Initialize()
{
    m_iAnimIndex = 32;
    m_eStateType = STATE_GETUP_ENEMY;




    m_fInterPolationTime = 0.05f;
    m_fStateChangableTime = 0.5f;
    m_fDelayTime = 0.55f;

    return S_OK;
}

void CGetUp_Enemy::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_bFront)
        m_iAnimIndex = 33;
    else
        m_iAnimIndex = 32;

    m_fAnimSpeed = 1.f;

    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator);

}

STATE_TYPE CGetUp_Enemy::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
    {
            return STATE_IDLE_ENEMY;

    }

    return __super::Tick(pOwner, pAnimator);
}

void CGetUp_Enemy::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */

}

STATE_TYPE CGetUp_Enemy::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Run으로 오는 조건
    1. 키눌렸을때
    */
    if (pAnimator->Is_CurAnimFinished())
        return m_eStateType;


    return STATE_END;
}
