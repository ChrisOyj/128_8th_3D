#include "stdafx.h"
#include "CWallJump_Go.h"

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

CWallJump_Go::CWallJump_Go()
{
}

CWallJump_Go::~CWallJump_Go()
{
}

CWallJump_Go* CWallJump_Go::Create()
{
    CWallJump_Go* pInstance = new CWallJump_Go();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWallJump_Go");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CWallJump_Go::Initialize()
{
    m_iAnimIndex = 77;
    m_eStateType = STATE_WALLJUMP_GO;

    m_vecAdjState.push_back(STATE_WALLJUMPEND_GO);

    m_fInterPolationTime = 0.1f;
    m_fStateChangableTime = 0.1f;
    return S_OK;
}

void CWallJump_Go::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
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

STATE_TYPE CWallJump_Go::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();

    pPhyscisCom->Set_Accel(30.f);

    if (pOwner->Get_Transform()->Get_World(WORLD_POS).y >= m_fTargetY)
        return m_vecAdjState.front();

    return __super::Tick(pOwner, pAnimator);
}

void CWallJump_Go::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();

        pPhyscisCom->Set_Dir(pOwner->Get_Transform()->Get_World(WORLD_LOOK));

}

STATE_TYPE CWallJump_Go::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
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
        _float4 vDir = pOwner->Get_Transform()->Get_World(WORLD_LOOK);
        vCellNormal.y = 0.f;
        vCellNormal.Normalize();
        _float fCosTheta = vCellNormal.Dot(vDir);

        if (fCosTheta < -0.8f)
            return m_eStateType;
    }


    return STATE_END;
}
