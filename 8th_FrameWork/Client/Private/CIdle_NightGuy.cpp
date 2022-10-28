#include "stdafx.h"
#include "CIdle_NightGuy.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CScript_FollowCam.h"
CIdle_NightGuy::CIdle_NightGuy()
{
}

CIdle_NightGuy::~CIdle_NightGuy()
{
}

CIdle_NightGuy* CIdle_NightGuy::Create()
{
    CIdle_NightGuy* pInstance = new CIdle_NightGuy();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CIdle_NightGuy");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CIdle_NightGuy::Initialize()
{
    m_eAnimType = ANIM_BASE;
    m_iAnimIndex = 36;
    m_eStateType = STATE_IDLE_NIGHTGUY;
    m_fInterPolationTime = 0.2f;


    m_vecAdjState.push_back(STATE_WALK_NIGHTGUY);
    m_vecAdjState.push_back(STATE_DASH_NIGHTGUY);
    m_vecAdjState.push_back(STATE_RUN_NIGHTGUY);
    m_vecAdjState.push_back(STATE_JUMP_NIGHTGUY);
    m_vecAdjState.push_back(STATE_ATTACK_1_NIGHTGUY);
    m_vecAdjState.push_back(STATE_NIGHTGUY_PLAYER);



    return S_OK;
}

void CIdle_NightGuy::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Owner의 Animator Set Idle로 */
    //여기서
    CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_DEFAULT);


    __super::Enter(pOwner, pAnimator);
}

void CIdle_NightGuy::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CIdle_NightGuy::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Idle로 오는 조건
    1. 현재 진행중인 애니메이션이 끝났을 때
    */
    if (pAnimator->Is_CurAnimFinished()
        && pOwner->Is_NightGuy())
        return m_eStateType;


    return STATE_END;
}
