#include "stdafx.h"
#include "CRun_End_NightGuy.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CScript_FollowCam.h"

#include "CEffects_Factory.h"
#include "CCell.h"
#include "Transform.h"
#include "CNavigation.h"
CRun_End_NightGuy::CRun_End_NightGuy()
{
}

CRun_End_NightGuy::~CRun_End_NightGuy()
{
}

CRun_End_NightGuy* CRun_End_NightGuy::Create()
{
    CRun_End_NightGuy* pInstance = new CRun_End_NightGuy();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CRun_End_NightGuy");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CRun_End_NightGuy::Initialize()
{
    m_iAnimIndex = 38;
    m_eStateType = STATE_RUN_END_NIGHTGUY;

    m_vecAdjState.push_back(STATE_IDLE_NIGHTGUY);
    m_vecAdjState.push_back(STATE_DASH_NIGHTGUY);
    m_vecAdjState.push_back(STATE_WALK_NIGHTGUY);
    m_vecAdjState.push_back(STATE_RUN_NIGHTGUY);
    m_vecAdjState.push_back(STATE_JUMP_NIGHTGUY);
    m_vecAdjState.push_back(STATE_ATTACK_1_NIGHTGUY);
    m_vecAdjState.push_back(STATE_NIGHTGUY_PLAYER);


    m_fInterPolationTime = 0.1f;
    m_fStateChangableTime = 0.1f;

    return S_OK;
}

void CRun_End_NightGuy::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator);
    CEffects_Factory::Get_Instance()->Create_LandingEffects(pOwner->Get_Transform()->Get_World(WORLD_POS), GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());

}

void CRun_End_NightGuy::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CRun_End_NightGuy::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
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
