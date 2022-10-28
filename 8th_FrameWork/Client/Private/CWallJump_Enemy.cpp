#include "stdafx.h"
#include "CWallJump_Enemy.h"

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

CWallJump_Enemy::CWallJump_Enemy()
{
}

CWallJump_Enemy::~CWallJump_Enemy()
{
}

CWallJump_Enemy* CWallJump_Enemy::Create()
{
    CWallJump_Enemy* pInstance = new CWallJump_Enemy();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWallJump_Enemy");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CWallJump_Enemy::Initialize()
{
    m_iAnimIndex = 77;
    m_eStateType = STATE_WALLJUMP_ENEMY;

    m_vecAdjState.push_back(STATE_WALLJUMPEND_ENEMY);

    m_fInterPolationTime = 0.1f;
    m_fStateChangableTime = 0.1f;
    return S_OK;
}

void CWallJump_Enemy::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    CFunctor::Play_Sound(L"Eff_WallJump", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));


    /* Owner의 Animator Set Idle로 */
    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();
    CTransform* pTransform = pOwner->Get_Transform();
    m_fTargetY = GET_COMPONENT_FROM(pOwner, CNavigation)->Get_TargetY();

    _float4 vLook = GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurWallNormal() * -1.f;
    vLook.y = 0.f;
    vLook.Normalize();
    pTransform->Set_Look(vLook);

    pPhyscisCom->Set_MaxSpeed(pOwner->Get_Status().fWallJumpSpeed);
    pPhyscisCom->Set_Dir(_float4(0.f, 1.f, 0.f, 0.f));
    pPhyscisCom->Set_Jump(pOwner->Get_Status().fJumpPower);

    __super::Enter(pOwner, pAnimator);


}

STATE_TYPE CWallJump_Enemy::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();

    pPhyscisCom->Set_Accel(30.f);

    if (pOwner->Get_Transform()->Get_World(WORLD_POS).y >= m_fTargetY)
        return m_vecAdjState.front();

    return __super::Tick(pOwner, pAnimator);
}

void CWallJump_Enemy::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();

    pPhyscisCom->Set_Dir(pOwner->Get_Transform()->Get_World(WORLD_LOOK));

}

STATE_TYPE CWallJump_Enemy::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Run으로 오는 조건
    1. 키눌렸을때
    */

    if (
        GET_COMPONENT_FROM(pOwner, CNavigation)->Is_OnWall()
        )
    {
        //벽의 법선벡터
        _float4 vCellNormal = GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurWallNormal();

        CUnit* pUnit = pOwner->Get_TargetUnit();

        _float4 vDir = pUnit->Get_Transform()->Get_World(WORLD_POS) - pOwner->Get_Transform()->Get_World(WORLD_POS);
        vDir.y = 0.f;
        vDir.Normalize();

        vCellNormal.y = 0.f;
        vCellNormal.Normalize();
        _float fCosTheta = vCellNormal.Dot(vDir);

        if (fCosTheta < -0.8f)
            return m_eStateType;
    }


    return STATE_END;
}
