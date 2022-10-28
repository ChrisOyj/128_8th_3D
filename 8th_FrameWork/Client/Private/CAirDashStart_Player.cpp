#include "stdafx.h"
#include "CAirDashStart_Player.h"

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
#include "Functor.h"
#include "CUnit_Player.h"

CAirDashStart_Player::CAirDashStart_Player()
{
}

CAirDashStart_Player::~CAirDashStart_Player()
{
}

CAirDashStart_Player* CAirDashStart_Player::Create()
{
    CAirDashStart_Player* pInstance = new CAirDashStart_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CAirDashStart_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CAirDashStart_Player::Initialize()
{
    m_iAnimIndex = 0;
    m_eStateType = STATE_AIRDASHSTART_PLAYER;

    m_vecAdjState.push_back(STATE_LAND_PLAYER);
    m_vecAdjState.push_back(STATE_FALL_PLAYER);
    m_vecAdjState.push_back(STATE_WALLJUMP_PLAYER);
    m_vecAdjState.push_back(STATE_DOUBLEJUMP_PLAYER);
    m_vecAdjState.push_back(STATE_NSAERIALATTACK_1_PLAYER);
    m_vecAdjState.push_back(STATE_HANDAERIALATTACK_1_PLAYER);
    m_vecAdjState.push_back(STATE_NSAERIALATTACK_3_PLAYER);
    m_vecAdjState.push_back(STATE_HANDAERIALATTACK_4_PLAYER);
    m_vecAdjState.push_back(STATE_THROW_PLAYER);
    m_vecAdjState.push_back(STATE_RASENSURIKEN_PLAYER);
    m_vecAdjState.push_back(STATE_CHIDORI);
    m_vecAdjState.push_back(STATE_FIREBALL_PLAYER);

    m_vecAdjState.push_back(STATE_WATERPRISON_PLAYER);
    //m_fInterPolationTime = 0.2f;
    m_fDelayTime = 0.3f;

    m_fInterPolationTime = 0.1f;
    m_fAnimSpeed = 1.7f;

    return S_OK;
}

void CAirDashStart_Player::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    //static_cast<CUnit_Player*>(pOwner)->TurnOn_AllTrailBuffer();
    CFunctor::Play_Sound(L"Eff_Dash", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));


    GET_STATE(m_eStateType, CAirDashStart_Player)->m_bCoolTime = false;
    pOwner->Disable_AllColliders();
    CEffects_Factory::Get_Instance()->Create_ScreenEffects(pOwner);


    _float fDashSpeed = pOwner->Get_Status().fDashSpeed * 2.f;

    CTransform* pTransform = pOwner->Get_Transform();
    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();

    pPhyscisCom->Get_PhysicsDetail().fFrictionRatio = 80.f;

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

    pPhyscisCom->Set_Jump(0.f);

    if (vDir.Is_Zero())
    {
        CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Dash_Effects_Air", pOwner, pTransform->Get_World(WORLD_POS));
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



    pPhyscisCom->Set_Dir(vTargetLook);

    __super::Enter(pOwner, pAnimator);


    /*CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();
    pPhyscisCom->Mul_Speed(0.5f);
    pPhyscisCom->Set_Jump(0.f);
    pPhyscisCom->Get_Physics().fGravity = 0.f;
    CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_ZOOM_RIGHT);


    __super::Enter(pOwner, pAnimator);*/

}

STATE_TYPE CAirDashStart_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_fTimeAcc > m_fDelayTime)
    {
        OnExecute(pOwner, pAnimator);

        return  STATE_FALL_PLAYER;
    }

    return __super::Tick(pOwner, pAnimator);
}

void CAirDashStart_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();

    pPhyscisCom->Get_Physics().fGravity = 9.8f;
    CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_DEFAULT);
    pPhyscisCom->Get_PhysicsDetail().fFrictionRatio = 1.f;

    pOwner->Enable_BodyColliders();
    //static_cast<CUnit_Player*>(pOwner)->TurnOff_AllTrailBuffer();

}

STATE_TYPE CAirDashStart_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Dash으로 오는 조건
    1. 키눌렸을때
    스킬
    스킬 쿨타임도 확인하고
    스킬 챠크라 게이지도 확인하고

    */
    if (pOwner->Get_PhysicsCom()->Get_Physics().bAir &&
        CUser::Get_Instance()->Get_LastKey() == KEY::LSHIFT)
    {
        return m_eStateType;
    }


    return STATE_END;
}

void CAirDashStart_Player::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();

    pPhyscisCom->Get_PhysicsDetail().fFrictionRatio = 1.f;

    pPhyscisCom->Get_Physics().fGravity = 9.8f;
    pPhyscisCom->Set_Jump(0.f);

    CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_DEFAULT);
    
}

void CAirDashStart_Player::TickExecute(CUnit* pOwner, CAnimator* pAnimator)
{
}
