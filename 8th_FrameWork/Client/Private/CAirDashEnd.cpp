#include "stdafx.h"
#include "CAirDashEnd.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "CCamera_Free.h"
#include "Physics.h"

#include "CUser.h"

#include "CScript_FollowCam.h"

CAirDashEnd::CAirDashEnd()
{
}

CAirDashEnd::~CAirDashEnd()
{
}

CAirDashEnd* CAirDashEnd::Create()
{
    CAirDashEnd* pInstance = new CAirDashEnd();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CAirDashEnd");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CAirDashEnd::Initialize()
{
    m_iAnimIndex = 0;
    m_eStateType = STATE_AIRDASHEND_PLAYER;

    m_vecAdjState.push_back(STATE_LAND_PLAYER);
    m_vecAdjState.push_back(STATE_FALL_PLAYER);
    m_vecAdjState.push_back(STATE_WALLJUMP_PLAYER);
    m_vecAdjState.push_back(STATE_DOUBLEJUMP_PLAYER);

    m_fInterPolationTime = 0.f;

    return S_OK;
}

void CAirDashEnd::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Enter(pOwner, pAnimator);

}

STATE_TYPE CAirDashEnd::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    CTransform* pTransform = pOwner->Get_Transform();
    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();

    _float4 vUserLook = CGameInstance::Get_Instance()->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
    vUserLook.y = 0.f;
    vUserLook.Normalize();

    _float4 _vLook = vUserLook;
    _float4 vUp = { 0.f, 1.f, 0.f, 0.f };
    _float4 vRight = vUp.Cross(_vLook);
    vUp = _vLook.Cross(vRight.Normalize());

    _float4 vDir = { 0.f, 0.f, 0.f, 0.f };
    if (KEY(A, HOLD))
    {
        vDir -= vRight;
    }
    if (KEY(D, HOLD))
    {
        vDir += vRight;
    }
    if (KEY(W, HOLD))
    {
        vDir += _vLook;
    }
    if (KEY(S, HOLD))
    {
        vDir -= _vLook;
    }

    if (vDir.Is_Zero())
    {
        return __super::Tick(pOwner, pAnimator);
    }

    _float4 vTargetLook = vDir.Normalize();

    _float4 vCurLook = pTransform->Get_World(WORLD_LOOK);
    _float fRatio = (vTargetLook - vCurLook).Length() * 4.f;
    vCurLook += (vTargetLook - vCurLook).Normalize() * fRatio * fDT;


    pTransform->Set_Look(vCurLook);
    return __super::Tick(pOwner, pAnimator);
}

void CAirDashEnd::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_DEFAULT);

}

STATE_TYPE CAirDashEnd::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
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