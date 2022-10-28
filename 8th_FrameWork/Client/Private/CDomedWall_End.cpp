#include "stdafx.h"
#include "CDomedWall_End.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "Transform.h"
#include "CScript_FollowCam.h"
CDomedWall_End::CDomedWall_End()
{
}

CDomedWall_End::~CDomedWall_End()
{
}

CDomedWall_End* CDomedWall_End::Create()
{
    CDomedWall_End* pInstance = new CDomedWall_End();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CDomedWall_End");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CDomedWall_End::Initialize()
{
    m_eAnimType = ANIM_WIN_TYPE_DANCE;
    m_iAnimIndex = 0;
    m_eStateType = STATE_DOMEDWALLEND_ENEMY;
    m_fInterPolationTime = 0.1f;
    m_fStateChangableTime = 7.5f;

    //적이 나보다 높으면 점프
    m_vecAdjState.push_back(STATE_JUMP_ENEMY);
    m_vecAdjState.push_back(STATE_RUN_ENEMY);
    m_vecAdjState.push_back(STATE_THROW_ENEMY);

    //공격범위 안이면 공격, 대쉬 혹은 가드
    m_vecAdjState.push_back(STATE_WALK_ENEMY);
    m_vecAdjState.push_back(STATE_DASH_ENEMY);
    m_vecAdjState.push_back(STATE_GUARD_ENEMY);
    m_vecAdjState.push_back(STATE_HANDATTACK_1_ENEMY);


    return S_OK;
}

void CDomedWall_End::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    _uint iRand = random(ANIM_WIN_TYPE_BOX, ANIM_WIN_TYPE_DANCE2);
    m_eAnimType = (ANIM_TYPE)iRand;

    __super::Enter(pOwner, pAnimator);
}

void CDomedWall_End::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CDomedWall_End::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
            return m_eStateType;
}
