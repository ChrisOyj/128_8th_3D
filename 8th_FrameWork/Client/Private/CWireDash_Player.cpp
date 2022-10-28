#include "stdafx.h"
#include "CWireDash_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "CCamera_Free.h"
#include "Physics.h"

#include "CUser.h"

#include "CScript_FollowCam.h"

#include "CState_Manager.h"
#include "CNavigation.h"

#include "CEffects_Factory.h"
#include "CScreenEffect.h"

#include "Functor.h"

CWireDash_Player::CWireDash_Player()
{
}

CWireDash_Player::~CWireDash_Player()
{
}

CWireDash_Player* CWireDash_Player::Create()
{
    CWireDash_Player* pInstance = new CWireDash_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWireDash_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CWireDash_Player::Initialize()
{
    m_iAnimIndex = 1;
    m_eStateType = STATE_WIREDASH_PLAYER;

    m_vecAdjState.push_back(STATE_LAND_PLAYER);
    m_vecAdjState.push_back(STATE_WALLJUMP_PLAYER);
    m_vecAdjState.push_back(STATE_DOUBLEJUMP_PLAYER);

    m_fInterPolationTime = 0.2f;

    return S_OK;
}

void CWireDash_Player::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 0.f;

    CFunctor::Play_Sound(L"Eff_Dash", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

    CEffects_Factory::Get_Instance()->Create_ScreenEffects(pOwner);
    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();

    if (pOwner->Is_NinzaSword())
    {
        if (pPhyscisCom->Get_Physics().bAir)
        {
            
        }

    }



    CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Dash_Effects_Air", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));


    CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_DASH);
    CState_Manager::Get_Instance()->Get_State(STATE_DOUBLEJUMP_PLAYER)->ReSet_CoolTime();


    pPhyscisCom->Set_MaxSpeed(pOwner->Get_Status().fWireSpeed);
    pPhyscisCom->Set_Speed(pOwner->Get_Status().fWireSpeed);

    __super::Enter(pOwner, pAnimator);

}

STATE_TYPE CWireDash_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        m_iCnt++;

    if (m_iCnt >= 2 || GET_COMPONENT_FROM(pOwner, CNavigation)->Is_OnWall())
    {
        return STATE_FALL_PLAYER;
    }

    return __super::Tick(pOwner, pAnimator);
}

void CWireDash_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    m_iCnt = 0;

    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();

    CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_DEFAULT);
    pPhyscisCom->Get_Physics().fGravity = 9.8f;
    pPhyscisCom->Set_Jump(0.f);
    pPhyscisCom->Mul_Speed(0.5f);
}

STATE_TYPE CWireDash_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return m_eStateType;

    return STATE_END;
}

void CWireDash_Player::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{

}

void CWireDash_Player::TickExecute(CUnit* pOwner, CAnimator* pAnimator)
{
}
