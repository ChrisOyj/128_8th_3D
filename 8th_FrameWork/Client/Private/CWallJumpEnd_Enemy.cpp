#include "stdafx.h"
#include "CWallJumpEnd_Enemy.h"

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

#include "CScript_FollowCam.h"

#include "Functor.h"

CWallJumpEnd_Enemy::CWallJumpEnd_Enemy()
{
}

CWallJumpEnd_Enemy::~CWallJumpEnd_Enemy()
{
}

CWallJumpEnd_Enemy* CWallJumpEnd_Enemy::Create()
{
    CWallJumpEnd_Enemy* pInstance = new CWallJumpEnd_Enemy();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWallJumpEnd_Enemy");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CWallJumpEnd_Enemy::Initialize()
{
    m_iAnimIndex = 29;
    m_eStateType = STATE_WALLJUMPEND_ENEMY;

    m_vecAdjState.push_back(STATE_FALL_ENEMY);
    m_vecAdjState.push_back(STATE_LAND_ENEMY);
    m_vecAdjState.push_back(STATE_WALLJUMP_ENEMY);

    m_fInterPolationTime = 0.1f;

    return S_OK;
}

void CWallJumpEnd_Enemy::Enter(CUnit* pOwner, CAnimator* pAnimator)
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

}

STATE_TYPE CWallJumpEnd_Enemy::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CWallJumpEnd_Enemy::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */

}

STATE_TYPE CWallJumpEnd_Enemy::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

    if (pAnimator->Is_CurAnimFinished())
    {
        return m_eStateType;
    }


    return STATE_END;
}
