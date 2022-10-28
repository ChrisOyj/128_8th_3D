#include "stdafx.h"
#include "CCJ_Loop_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CState_Manager.h"
#include "CCJ_End_Player.h"

#include "CUser.h"
#include "CScript_FollowCam.h"

#include "CChakraJump_Guide.h"
#include "CEffects_Factory.h"
#include "Transform.h"
#include "Functor.h"
CCJ_Loop_Player::CCJ_Loop_Player()
{
}

CCJ_Loop_Player::~CCJ_Loop_Player()
{
}

CCJ_Loop_Player* CCJ_Loop_Player::Create()
{
    CCJ_Loop_Player* pInstance = new CCJ_Loop_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CCJ_Loop_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CCJ_Loop_Player::Initialize()
{
    m_iAnimIndex = 17;
    m_eStateType = STATE_CJ_LOOP_PLAYER;
    m_fInterPolationTime = 0.f;


    m_vecAdjState.push_back(STATE_CJ_END_PLAYER);
    m_vecAdjState.push_back(STATE_DASH_PLAYER);

    m_fChargeMaxTime = 1.5f;

    m_fDelayTime = m_fChargeMaxTime;

    m_wstrSoundKey = L"Eff_ChakraJumpLoop";
    m_eChannel = CHANNEL_EFFECTS;
    m_fSoundLoopTime = 0.5f;


    return S_OK;
}

void CCJ_Loop_Player::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Owner의 Animator Set Idle로 */
    //여기서
  //  CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"ChakraParticle"), pOwner->Get_Transform()->Get_World(WORLD_POS));
    CFunctor::Play_Sound(L"Eff_ChakraJumpLoop", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

    __super::Enter(pOwner, pAnimator);


}

void CCJ_Loop_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
    static_cast<CCJ_End_Player*>(CState_Manager::Get_Instance()->Get_State(STATE_CJ_END_PLAYER))
        ->Set_ChargeTime(min(m_fTimeAcc, m_fChargeMaxTime));


}

STATE_TYPE CCJ_Loop_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Idle로 오는 조건
    1. 현재 진행중인 애니메이션이 끝났을 때
    */
    if (pAnimator->Is_CurAnimFinished())
        return m_eStateType;


    return (STATE_TYPE)STATE_END;
}

void CCJ_Loop_Player::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
    _float4 vPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
    CEffects_Factory::Get_Instance()->Create_MultiEffects(L"CJ_MaxCharge", pOwner, vPos);
    CFunctor::Play_Sound(L"Eff_ChakraJumpMaxCharge", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

}
