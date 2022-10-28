#include "stdafx.h"
#include "CCJ_Start_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CScript_FollowCam.h"

#include "CChakraJump_Guide.h"
#include "CEffects_Factory.h"
#include "Transform.h"
#include "Functor.h"

CCJ_Start_Player::CCJ_Start_Player()
{
}

CCJ_Start_Player::~CCJ_Start_Player()
{
}

CCJ_Start_Player* CCJ_Start_Player::Create()
{
    CCJ_Start_Player* pInstance = new CCJ_Start_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CCJ_Start_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CCJ_Start_Player::Initialize()
{
    m_iAnimIndex = 18;
    m_eStateType = STATE_CJ_START_PLAYER;
    m_fInterPolationTime = 0.15f;
    m_fDelayTime = 0.3f;


    m_vecAdjState.push_back(STATE_CJ_LOOP_PLAYER);
    m_vecAdjState.push_back(STATE_DASH_PLAYER);


    return S_OK;
}

void CCJ_Start_Player::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Owner의 Animator Set Idle로 */
    //여기서

    __super::Enter(pOwner, pAnimator);
    CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_ZOOM_RIGHT);
}

STATE_TYPE CCJ_Start_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (KEY(C, NONE))
        return STATE_IDLE_PLAYER;

    return __super::Tick(pOwner, pAnimator);
}

void CCJ_Start_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CCJ_Start_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Idle로 오는 조건
    1. 현재 진행중인 애니메이션이 끝났을 때
    */
    if (CUser::Get_Instance()->Get_LastKey() == KEY::C)
        return m_eStateType;


    return STATE_END;
}

void CCJ_Start_Player::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
    CFunctor::Play_Sound(L"Eff_ChakraJumpStart", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

    _float4 vPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
    CEffects_Factory::Get_Instance()->Create_MultiEffects(L"CJ_Start", pOwner, vPos);
    CEffects_Factory::Get_Instance()->Create_Effects(HASHCODE(CChakraJump_Guide), vPos);


}
