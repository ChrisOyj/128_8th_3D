#include "stdafx.h"
#include "CWalk_End_NightGuy.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"


CWalk_End_NightGuy::CWalk_End_NightGuy()
{
}

CWalk_End_NightGuy::~CWalk_End_NightGuy()
{
}

CWalk_End_NightGuy* CWalk_End_NightGuy::Create()
{
    CWalk_End_NightGuy* pInstance = new CWalk_End_NightGuy();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWalk_End_NightGuy");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CWalk_End_NightGuy::Initialize()
{
    m_iAnimIndex = 39;
    m_eStateType = STATE_WALK_END_NIGHTGUY;

    m_vecAdjState.push_back(STATE_IDLE_NIGHTGUY);
    m_vecAdjState.push_back(STATE_WALK_NIGHTGUY);
    m_vecAdjState.push_back(STATE_DASH_NIGHTGUY);
    m_vecAdjState.push_back(STATE_RUN_NIGHTGUY);
    m_vecAdjState.push_back(STATE_JUMP_NIGHTGUY);
    m_vecAdjState.push_back(STATE_ATTACK_1_NIGHTGUY);
    m_vecAdjState.push_back(STATE_NIGHTGUY_PLAYER);


    m_fInterPolationTime = 0.1f;

    return S_OK;
}

void CWalk_End_NightGuy::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator);
}

void CWalk_End_NightGuy::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CWalk_End_NightGuy::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 WalkEnd로 오는 조건
    1. 키가 안눌렸을 때
    */
    if (KEY(W, NONE) &&
        KEY(A, NONE) &&
        KEY(S, NONE) &&
        KEY(D, NONE))
    {
        return m_eStateType;
    }


    return STATE_END;
}
