#include "stdafx.h"
#include "CWallHitEnd_Enemy.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "CCamera_Free.h"
#include "Physics.h"

#include "CState_Manager.h"
#include "CDoubleJump_Player.h"

#include "CNavigation.h"

#include "CUser.h"
#include "CHit_Fall.h"
#include "CDown_Enemy.h"
#include "CGetUp_Enemy.h"
#include "CScript_FollowCam.h"

CWallHitEnd_Enemy::CWallHitEnd_Enemy()
{
}

CWallHitEnd_Enemy::~CWallHitEnd_Enemy()
{
}

CWallHitEnd_Enemy* CWallHitEnd_Enemy::Create()
{
    CWallHitEnd_Enemy* pInstance = new CWallHitEnd_Enemy();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWallHitEnd_Enemy");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CWallHitEnd_Enemy::Initialize()
{
    m_eAnimType = ANIM_BEATEN;
    m_iAnimIndex = 51;
    m_eStateType = STATE_WALLHITEND_ENEMY;

    m_vecAdjState.push_back(STATE_HIT_FALL_ENEMY);
    m_vecAdjState.push_back(STATE_DOWN_ENEMY);

    m_fInterPolationTime = 0.05f;

    return S_OK;
}

void CWallHitEnd_Enemy::Enter(CUnit* pOwner, CAnimator* pAnimator)
{

    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();
    pPhyscisCom->Set_Jump(0.f);
    //pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 0.f;

    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator);

}

STATE_TYPE CWallHitEnd_Enemy::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);
}

void CWallHitEnd_Enemy::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
    _float4 vLook = pOwner->Get_Transform()->Get_World(WORLD_LOOK);

    static_cast<CHit_Fall*>(CState_Manager::Get_Instance()->Get_State(STATE_HIT_FALL_ENEMY))->On_HitFall(
        CHit_Fall::FRONT, vLook * -1.f, vLook, 0.f, 0.f, 0.f);

    static_cast<CDown_Enemy*>(CState_Manager::Get_Instance()->Get_State(STATE_DOWN_ENEMY))->Set_Front(
       true
    );

    static_cast<CGetUp_Enemy*>(CState_Manager::Get_Instance()->Get_State(STATE_GETUP_ENEMY))->Set_Front(
        true
    );

    //pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 9.8f;

}

STATE_TYPE CWallHitEnd_Enemy::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Run으로 오는 조건
    1. 키눌렸을때
    */
    if (pAnimator->Is_CurAnimFinished())
        return m_eStateType;


    return STATE_END;
}
