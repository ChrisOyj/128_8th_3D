#include "stdafx.h"
#include "CWallJumpEnd_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "CCamera_Free.h"
#include "Physics.h"

#include "CUser.h"
#include "CNavigation.h"

#include "CDoubleJump_Player.h"
#include "CState_Manager.h"
#include "Functor.h"
#include "CScript_FollowCam.h"

CWallJumpEnd_Player::CWallJumpEnd_Player()
{
}

CWallJumpEnd_Player::~CWallJumpEnd_Player()
{
}

CWallJumpEnd_Player* CWallJumpEnd_Player::Create()
{
    CWallJumpEnd_Player* pInstance = new CWallJumpEnd_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWallJumpEnd_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CWallJumpEnd_Player::Initialize()
{
    m_iAnimIndex = 29;
    m_eStateType = STATE_WALLJUMPEND_PLAYER;

    m_vecAdjState.push_back(STATE_FALL_PLAYER);
    m_vecAdjState.push_back(STATE_LAND_PLAYER);
    m_vecAdjState.push_back(STATE_DOUBLEJUMP_PLAYER);
    m_vecAdjState.push_back(STATE_AIRDASHSTART_PLAYER);

    m_fInterPolationTime = 0.1f;

    return S_OK;
}

void CWallJumpEnd_Player::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    CFunctor::Play_Sound(L"Eff_Jump", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator);
    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();
    CTransform* pTransform = pOwner->Get_Transform();

    pPhyscisCom->Set_Jump(pOwner->Get_Status().fJumpPower * 1.2f);
    pPhyscisCom->Set_MaxSpeed(pOwner->Get_Status().fRunSpeed);
    pPhyscisCom->Set_Speed(pOwner->Get_Status().fRunSpeed);

    //가는방향 바라보게
    pTransform->Set_Look(pPhyscisCom->Get_Physics().vDir);

    CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_DEFAULT);
}

STATE_TYPE CWallJumpEnd_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CWallJumpEnd_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */

}

STATE_TYPE CWallJumpEnd_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Run으로 오는 조건
    1. 키눌렸을때
    */


    if (pAnimator->Is_CurAnimFinished())
    {
        return m_eStateType;
    }


    return STATE_END;
}
