#include "stdafx.h"
#include "CDash_NightGuy.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "CCamera_Free.h"
#include "Physics.h"

#include "CUser.h"

#include "CScript_FollowCam.h"
#include "CState_Manager.h"

#include "CEffects_Factory.h"
#include "CScreenEffect.h"

#include "Renderer.h"

#include "Functor.h"

CDash_NightGuy::CDash_NightGuy()
{
}

CDash_NightGuy::~CDash_NightGuy()
{
}

CDash_NightGuy* CDash_NightGuy::Create()
{
    CDash_NightGuy* pInstance = new CDash_NightGuy();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CDash_NightGuy");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CDash_NightGuy::Initialize()
{
    m_iAnimIndex = 0;
    m_eStateType = STATE_DASH_NIGHTGUY;

    m_vecAdjState.push_back(STATE_LAND_NIGHTGUY);

    m_vecAdjState.push_back(STATE_WALLJUMP_NIGHTGUY);
    m_vecAdjState.push_back(STATE_DOUBLEJUMP_NIGHTGUY);
    m_vecAdjState.push_back(STATE_ATTACK_1_NIGHTGUY);


    m_fDelayTime = 0.15f;

    m_fInterPolationTime = 0.1f;
    m_fAnimSpeed = 1.7f;

    return S_OK;
}

void CDash_NightGuy::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    //static_cast<CUnit_NightGuy*>(pOwner)->TurnOn_AllTrailBuffer();
    CFunctor::Play_Sound(L"Eff_Dash", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

    DISABLE_COMPONENT(GET_COMPONENT_FROM(pOwner, CRenderer));

    pOwner->Disable_AllColliders();

    CEffects_Factory::Get_Instance()->Create_ScreenEffects(pOwner);


    _float fDashSpeed = pOwner->Get_Status().fDashSpeed * 3.f;

    CTransform* pTransform = pOwner->Get_Transform();
    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();

    pPhyscisCom->Get_PhysicsDetail().fFrictionRatio = 230.f;

    _float4 vUserLook = CGameInstance::Get_Instance()->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
    vUserLook.Normalize();

    _float4 _vLook = vUserLook;
    _float4 vUp = pTransform->Get_World(WORLD_UP);
    _float4 vRight = vUp.Cross(_vLook);
    _vLook = vRight.Cross(vUp.Normalize());
    _vLook.Normalize();

    _float4 vDir = { 0.f, 0.f, 0.f, 0.f };
    if (KEY(W, HOLD))
    {
        vDir += _vLook;
        CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_BACKDASH);

    }
    if (KEY(A, HOLD))
    {
        vDir -= vRight;
        CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_RIGHT);


    }
    if (KEY(D, HOLD))
    {
        vDir += vRight;
        CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_LEFT);

    }

    if (KEY(S, HOLD))
    {
        vDir -= _vLook;
        CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_BACKDASH);

    }

    pPhyscisCom->Set_MaxSpeed(fDashSpeed);
    pPhyscisCom->Set_Speed(fDashSpeed);

    pPhyscisCom->Get_Physics().fGravity = 0.f;

    pPhyscisCom->Set_Jump(2.f);

    if (vDir.Is_Zero())
    {
        CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Dash_Effects_Air", pOwner, pTransform->Get_World(WORLD_POS));
        CEffects_Factory::Get_Instance()->Create_MultiEffects(L"EightGatesDash", pOwner, pTransform->Get_World(WORLD_POS));
        __super::Enter(pOwner, pAnimator);
        return;
    }

    _float4 vTargetLook = vDir.Normalize();
    _float4 vCurLook = pTransform->Get_World(WORLD_LOOK);

    vCurLook = vTargetLook;

    pTransform->Set_LerpLook(vCurLook, 0.3f);

    pTransform->Set_Look(vCurLook);
    pTransform->Make_WorldMatrix();
    CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Dash_Effects_Air", pOwner, pTransform->Get_World(WORLD_POS));
    CEffects_Factory::Get_Instance()->Create_MultiEffects(L"EightGatesDash", pOwner, pTransform->Get_World(WORLD_POS));



    pPhyscisCom->Set_Dir(vTargetLook);

    __super::Enter(pOwner, pAnimator);


    /*CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();
    pPhyscisCom->Mul_Speed(0.5f);
    pPhyscisCom->Set_Jump(0.f);
    pPhyscisCom->Get_Physics().fGravity = 0.f;
    CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_ZOOM_RIGHT);


    __super::Enter(pOwner, pAnimator);*/

}

STATE_TYPE CDash_NightGuy::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_fTimeAcc > m_fDelayTime)
    {
        OnExecute(pOwner, pAnimator);

        return  STATE_FALL_NIGHTGUY;
    }

    return __super::Tick(pOwner, pAnimator);
}

void CDash_NightGuy::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();

    pPhyscisCom->Get_Physics().fGravity = 9.8f;
    CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_DEFAULT);

    pOwner->Enable_BodyColliders();
    //static_cast<CUnit_NightGuy*>(pOwner)->TurnOff_AllTrailBuffer();
    ENABLE_COMPONENT(GET_COMPONENT_FROM(pOwner, CRenderer));

    pPhyscisCom->Get_PhysicsDetail().fFrictionRatio = 1.f;
    pPhyscisCom->Set_MaxSpeed(7.f);


}

STATE_TYPE CDash_NightGuy::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Dash으로 오는 조건
    1. 키눌렸을때
    스킬
    스킬 쿨타임도 확인하고
    스킬 챠크라 게이지도 확인하고

    */
    if ( CUser::Get_Instance()->Get_LastKey() == KEY::LSHIFT
        && pOwner->Is_NightGuy())
    {
        return m_eStateType;
    }


    return STATE_END;
}

void CDash_NightGuy::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
    ENABLE_COMPONENT(GET_COMPONENT_FROM(pOwner, CRenderer));

    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();

    pPhyscisCom->Get_Physics().fGravity = 9.8f;
    pPhyscisCom->Set_Jump(2.f);


    
}

void CDash_NightGuy::TickExecute(CUnit* pOwner, CAnimator* pAnimator)
{
}
