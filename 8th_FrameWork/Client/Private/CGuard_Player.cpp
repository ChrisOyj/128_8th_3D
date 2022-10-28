#include "stdafx.h"
#include "CGuard_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CScript_FollowCam.h"
#include "Functor.h"
#include "Transform.h"
CGuard_Player::CGuard_Player()
{
}

CGuard_Player::~CGuard_Player()
{
}

CGuard_Player* CGuard_Player::Create()
{
    CGuard_Player* pInstance = new CGuard_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CGuard_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CGuard_Player::Initialize()
{
    m_eAnimType = ANIM_GUARD;
    m_iAnimIndex = 14;

    m_eStateType = STATE_GUARD_PLAYER;
    m_fInterPolationTime = 0.1f;


    m_vecAdjState.push_back(STATE_DASH_PLAYER);
    m_vecAdjState.push_back(STATE_JUMP_PLAYER);
    m_vecAdjState.push_back(STATE_CJ_START_PLAYER);


    return S_OK;
}

void CGuard_Player::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Owner의 Animator Set Idle로 */
    //여기서
    CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_DEFAULT);

    CFunctor::Play_Sound(L"Voice_Man_Attack", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));

    __super::Enter(pOwner, pAnimator);
}

STATE_TYPE CGuard_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (KEY(E, NONE))
    {
        return STATE_IDLE_PLAYER;
    }

    return __super::Tick(pOwner, pAnimator);
}

void CGuard_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CGuard_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Idle로 오는 조건
    1. 현재 진행중인 애니메이션이 끝났을 때
    */
    if (KEY(E, HOLD))
    {
        return m_eStateType;
    }


    return STATE_END;
}
