#include "stdafx.h"
#include "CGetUp_Player.h"

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

CGetUp_Player::CGetUp_Player()
{
}

CGetUp_Player::~CGetUp_Player()
{
}

CGetUp_Player* CGetUp_Player::Create()
{
    CGetUp_Player* pInstance = new CGetUp_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CGetUp_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CGetUp_Player::Initialize()
{
    m_iAnimIndex = 32;
    m_eStateType = STATE_GETUP_PLAYER;


    m_vecAdjState.push_back(STATE_IDLE_NIGHTGUY);
    m_vecAdjState.push_back(STATE_DASH_NIGHTGUY);

    m_vecAdjState.push_back(STATE_IDLE_PLAYER);
    m_vecAdjState.push_back(STATE_DASH_PLAYER);

    m_fInterPolationTime = 0.05f;
    m_fStateChangableTime = 0.5f;

    return S_OK;
}

void CGetUp_Player::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_bFront)
        m_iAnimIndex = 33;
    else
        m_iAnimIndex = 32;

    m_fAnimSpeed = 1.f;

    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator);

}

STATE_TYPE CGetUp_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);
}

void CGetUp_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */

}

STATE_TYPE CGetUp_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Run으로 오는 조건
    1. 키눌렸을때
    */
    if (pAnimator->Is_CurAnimFinished())
        return m_eStateType;


    return STATE_END;
}
