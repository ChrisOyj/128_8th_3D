#include "stdafx.h"
#include "CAirDash_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "CCamera_Free.h"
#include "Physics.h"

#include "CUser.h"

#include "CScript_FollowCam.h"

CAirDash_Player::CAirDash_Player()
{
}

CAirDash_Player::~CAirDash_Player()
{
}

CAirDash_Player* CAirDash_Player::Create()
{
    CAirDash_Player* pInstance = new CAirDash_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CAirDash_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CAirDash_Player::Initialize()
{
    m_iAnimIndex = 1;
    m_eStateType = STATE_AIRDASHLOOP_PLAYER;

    m_vecAdjState.push_back(STATE_LAND_PLAYER);
    m_vecAdjState.push_back(STATE_AIRDASHEND_PLAYER);
    m_vecAdjState.push_back(STATE_WALLJUMP_PLAYER);
    m_vecAdjState.push_back(STATE_DOUBLEJUMP_PLAYER);

    m_fInterPolationTime = 0.f;

    return S_OK;
}

void CAirDash_Player::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    

    __super::Enter(pOwner, pAnimator);

}

STATE_TYPE CAirDash_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);
}

void CAirDash_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_DEFAULT);

    pOwner->Get_PhysicsCom()->Set_Speed(pOwner->Get_PhysicsCom()->Get_Physics().fSpeed * 0.7f);
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;

}

STATE_TYPE CAirDash_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Dash으로 오는 조건
    1. 키눌렸을때
    스킬
    스킬 쿨타임도 확인하고
    스킬 챠크라 게이지도 확인하고

    */
    if (pAnimator->Is_CurAnimFinished())
    {
        return m_eStateType;
    }


    return STATE_END;
}