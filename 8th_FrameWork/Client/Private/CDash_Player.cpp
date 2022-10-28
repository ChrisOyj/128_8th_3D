#include "stdafx.h"
#include "CDash_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "CCamera_Free.h"
#include "Physics.h"
#include "CScreenEffect.h"
#include "CUser.h"

#include "CScript_FollowCam.h"
#include "CEffects_Factory.h"
#include "CCell.h"
#include "CNavigation.h"

#include "CUnit_Player.h"
#include "Functor.h"
CDash_Player::CDash_Player()
{
}

CDash_Player::~CDash_Player()
{
}

CDash_Player* CDash_Player::Create()
{
    CDash_Player* pInstance = new CDash_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CDash_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CDash_Player::Initialize()
{
    m_iAnimIndex = 24;
    m_eStateType = STATE_DASH_PLAYER;
    

    m_vecAdjState.push_back(STATE_IDLE_PLAYER);
    m_vecAdjState.push_back(STATE_DASH_PLAYER);
    m_vecAdjState.push_back(STATE_RUN_PLAYER);
    m_vecAdjState.push_back(STATE_JUMP_PLAYER);
    m_vecAdjState.push_back(STATE_CJ_START_PLAYER);
    m_vecAdjState.push_back(STATE_NSATTACK_1_PLAYER);
    m_vecAdjState.push_back(STATE_HANDATTACK_DASH_PLAYER);
    m_vecAdjState.push_back(STATE_HANDATTACK_POWER_PLAYER);
    m_vecAdjState.push_back(STATE_GUARD_PLAYER);
    m_vecAdjState.push_back(STATE_THROW_PLAYER);
    m_vecAdjState.push_back(STATE_RASENSURIKEN_PLAYER);
    m_vecAdjState.push_back(STATE_RASENGAN);
    m_vecAdjState.push_back(STATE_KAMUI_PLAYER);
    m_vecAdjState.push_back(STATE_EIGHTGATES);
    m_vecAdjState.push_back(STATE_CHIDORI);
    m_vecAdjState.push_back(STATE_FIREBALL_PLAYER);

    m_vecAdjState.push_back(STATE_WATERPRISON_PLAYER);
    m_vecAdjState.push_back(STATE_SHARKBOMB_PLAYER);
    m_vecAdjState.push_back(STATE_WATERFALL_PLAYER);

    m_fInterPolationTime = 0.1f;
    m_fStateChangableTime = 0.75f;
    m_fAnimSpeed = 1.1f;
    m_fDelayTime = 0.4f;

    return S_OK;
}

void CDash_Player::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    CFunctor::Play_Sound(L"Eff_GroundDash", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

    CEffects_Factory::Get_Instance()->Create_LandingEffects(pOwner->Get_Transform()->Get_World(WORLD_POS), GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());
    
    //CEffects_Factory::Get_Instance()->Create_ScreenEffects(pOwner);
    //static_cast<CUnit_Player*>(pOwner)->TurnOn_AllTrailBuffer();


    pOwner->Disable_AllColliders();

    m_iAnimIndex = 24;
    m_fStateChangableTime = 0.75f;

    _float fDashSpeed = pOwner->Get_Status().fDashSpeed;

    

    CTransform* pTransform = pOwner->Get_Transform();
    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();

    _float4 vUserLook = CGameInstance::Get_Instance()->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
    vUserLook.Normalize();

    _float4 _vLook = vUserLook;
    _float4 vUp = pTransform->Get_World(WORLD_UP);
    _float4 vRight = vUp.Cross(_vLook);
    _vLook = vRight.Cross(vUp.Normalize());
    _vLook.Normalize();
    CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_BACKDASH);

    _float4 vDir = { 0.f, 0.f, 0.f, 0.f };
    if (KEY(W, HOLD))
    {

        vDir += _vLook;
        m_iAnimIndex = 24;
        fDashSpeed = pOwner->Get_Status().fDashSpeed * 0.8f;
        m_fStateChangableTime = 0.3f;
    }
    if (KEY(A, HOLD))
    {
        vDir -= vRight;
        m_iAnimIndex = 25;
        CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_RIGHT);
        m_fStateChangableTime = 0.6f;


    }
    if (KEY(D, HOLD))
    {
        vDir += vRight;
        m_iAnimIndex = 26;
        CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_LEFT);
        m_fStateChangableTime = 0.6f;

    }
    
    if (KEY(S, HOLD))
    {
        vDir -= _vLook;
        m_iAnimIndex = 23;

    }

    pPhyscisCom->Set_MaxSpeed(fDashSpeed);
    pPhyscisCom->Set_Speed(fDashSpeed);
    //pPhyscisCom->Set_Jump(2.f);

   

    if (vDir.Is_Zero())
    {
        if (m_iAnimIndex == 24)
        {
            CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Dash_Effects_Ground", pOwner, pTransform->Get_World(WORLD_POS));
        }
        __super::Enter(pOwner, pAnimator);
        return;
    }

    _float4 vTargetLook = vDir.Normalize();
    _float4 vCurLook = pTransform->Get_World(WORLD_LOOK);

    vCurLook = _vLook;

    pTransform->Set_LerpLook(vCurLook, 0.4f);

    if (m_iAnimIndex == 24)
    {
        pTransform->Set_Look(vDir);
        pTransform->Make_WorldMatrix();
        CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Dash_Effects_Ground", pOwner, pTransform->Get_World(WORLD_POS));
    }
  

    pPhyscisCom->Set_Dir(vTargetLook);
    pPhyscisCom->Get_PhysicsDetail().fFrictionRatio = 0.5f;

    __super::Enter(pOwner, pAnimator);

}

STATE_TYPE CDash_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (0.4f <= m_fTimeAcc + fDT && 
        pOwner->Get_PhysicsCom()->Get_Physics().bAir)
    {
        return STATE_FALL_PLAYER;
    }

    return __super::Tick(pOwner, pAnimator);
}

void CDash_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
    CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_DEFAULT);

    if (pOwner->Get_PhysicsCom()->Get_Physics().bAir)
        pOwner->Get_PhysicsCom()->Set_Speed(pOwner->Get_PhysicsCom()->Get_Physics().fSpeed * 0.5f);

    pOwner->Enable_BodyColliders();
    //static_cast<CUnit_Player*>(pOwner)->TurnOff_AllTrailBuffer();

}

STATE_TYPE CDash_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Dash으로 오는 조건
    1. 키눌렸을때
    스킬
    스킬 쿨타임도 확인하고
    스킬 챠크라 게이지도 확인하고

    */
    if (CUser::Get_Instance()->Get_LastKey() == KEY::LSHIFT)
    {
      return m_eStateType;
    }


    return STATE_END;
}

void CDash_Player::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->Enable_BodyColliders();
}
