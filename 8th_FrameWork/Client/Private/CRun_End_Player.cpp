#include "stdafx.h"
#include "CRun_End_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CScript_FollowCam.h"

#include "CEffects_Factory.h"
#include "CCell.h"
#include "Transform.h"
#include "CNavigation.h"
CRun_End_Player::CRun_End_Player()
{
}

CRun_End_Player::~CRun_End_Player()
{
}

CRun_End_Player* CRun_End_Player::Create()
{
    CRun_End_Player* pInstance = new CRun_End_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CRun_End_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CRun_End_Player::Initialize()
{
    m_iAnimIndex = 38;
    m_eStateType = STATE_RUN_END_PLAYER;

    m_vecAdjState.push_back(STATE_IDLE_PLAYER);
    m_vecAdjState.push_back(STATE_WALK_PLAYER);
    m_vecAdjState.push_back(STATE_DASH_PLAYER);
    m_vecAdjState.push_back(STATE_RUN_PLAYER);
    m_vecAdjState.push_back(STATE_JUMP_PLAYER);
    m_vecAdjState.push_back(STATE_CJ_START_PLAYER);
    m_vecAdjState.push_back(STATE_NSATTACK_1_PLAYER);
    m_vecAdjState.push_back(STATE_HANDATTACK_1_PLAYER);
    m_vecAdjState.push_back(STATE_GUARD_PLAYER);
    m_vecAdjState.push_back(STATE_HANDATTACK_POWER_PLAYER);
    m_vecAdjState.push_back(STATE_THROW_PLAYER);
    m_vecAdjState.push_back(STATE_RASENSURIKEN_PLAYER);
    m_vecAdjState.push_back(STATE_RASENGAN);
    m_vecAdjState.push_back(STATE_KAMUI_PLAYER);
    m_vecAdjState.push_back(STATE_EIGHTGATES);
    m_vecAdjState.push_back(STATE_CHIDORI);
    m_vecAdjState.push_back(STATE_FIREBALL_PLAYER);
    m_vecAdjState.push_back(STATE_WATERPRISON_PLAYER);
    m_vecAdjState.push_back(STATE_SHARKBOMB_PLAYER);
    m_vecAdjState.push_back(STATE_WATERFALL_PLAYER);


    m_fInterPolationTime = 0.1f;
    m_fStateChangableTime = 0.1f;

    return S_OK;
}

void CRun_End_Player::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator);
    CEffects_Factory::Get_Instance()->Create_LandingEffects(pOwner->Get_Transform()->Get_World(WORLD_POS), GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());

}

void CRun_End_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CRun_End_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
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
