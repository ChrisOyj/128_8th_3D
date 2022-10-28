#include "stdafx.h"
#include "CWalk_End_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"


CWalk_End_Player::CWalk_End_Player()
{
}

CWalk_End_Player::~CWalk_End_Player()
{
}

CWalk_End_Player* CWalk_End_Player::Create()
{
    CWalk_End_Player* pInstance = new CWalk_End_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWalk_End_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CWalk_End_Player::Initialize()
{
    m_iAnimIndex = 39;
    m_eStateType = STATE_WALK_END_PLAYER;
    m_vecAdjState.push_back(STATE_IDLE_PLAYER);
    m_vecAdjState.push_back(STATE_DASH_PLAYER);
    m_vecAdjState.push_back(STATE_WALK_PLAYER);
    m_vecAdjState.push_back(STATE_JUMP_PLAYER);
    m_vecAdjState.push_back(STATE_RUN_PLAYER);
    m_vecAdjState.push_back(STATE_CJ_START_PLAYER);
    m_vecAdjState.push_back(STATE_NSATTACK_1_PLAYER);
    m_vecAdjState.push_back(STATE_HANDATTACK_1_PLAYER);
    m_vecAdjState.push_back(STATE_HANDATTACK_POWER_PLAYER);
    m_vecAdjState.push_back(STATE_GUARD_PLAYER);
    m_vecAdjState.push_back(STATE_RASENSURIKEN_PLAYER);
    m_vecAdjState.push_back(STATE_RASENGAN);
    m_vecAdjState.push_back(STATE_KAMUI_PLAYER);
    m_vecAdjState.push_back(STATE_CHIDORI);
    m_vecAdjState.push_back(STATE_EIGHTGATES);
    m_vecAdjState.push_back(STATE_FIREBALL_PLAYER);

    m_vecAdjState.push_back(STATE_WATERPRISON_PLAYER);
    m_vecAdjState.push_back(STATE_SHARKBOMB_PLAYER);
    m_vecAdjState.push_back(STATE_WATERFALL_PLAYER);

    m_fInterPolationTime = 0.1f;

    return S_OK;
}

void CWalk_End_Player::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator);
}

void CWalk_End_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CWalk_End_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
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
