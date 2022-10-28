#include "stdafx.h"
#include "CGuardHit_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "Physics.h"
#include "Transform.h"
#include "CScript_FollowCam.h"
#include "CGuard.h"
#include "CEffects_Factory.h"
#include "CCell.h"
#include "CNavigation.h"

#include "Functor.h"
CGuardHit_Player::CGuardHit_Player()
{
}

CGuardHit_Player::~CGuardHit_Player()
{
}

CGuardHit_Player* CGuardHit_Player::Create()
{
    CGuardHit_Player* pInstance = new CGuardHit_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CGuardHit_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

void CGuardHit_Player::On_GuardHit(_float4 vLook, TYPE eType)
{
    m_vLook = vLook;
    m_eType = eType;
}

HRESULT CGuardHit_Player::Initialize()
{
    m_eAnimType = ANIM_GUARD;
    m_iAnimIndex = 7;

    m_eStateType = STATE_GUARDHIT_PLAYER;
    m_fInterPolationTime = 0.05f;
    m_fStateChangableTime = 0.2f;


    m_vecAdjState.push_back(STATE_WALK_PLAYER);
    m_vecAdjState.push_back(STATE_DASH_PLAYER);
    m_vecAdjState.push_back(STATE_HANDATTACK_DASH_PLAYER);
    m_vecAdjState.push_back(STATE_RUN_PLAYER);
    m_vecAdjState.push_back(STATE_JUMP_PLAYER);
    m_vecAdjState.push_back(STATE_RASENSURIKEN_PLAYER);


    return S_OK;
}

void CGuardHit_Player::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    CFunctor::Play_Sound(L"Eff_Hit_Sword", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

    CEffects_Factory::Get_Instance()->Create_LandingEffects(pOwner->Get_Transform()->Get_World(WORLD_POS), GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());

    CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"GuardHit_0"), pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));

    CEffects_Factory::Get_Instance()->Create_Effects(HASHCODE(CGuard), pOwner);


    CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_BACKDASH);
    _float fSpeed;
    switch (m_eType)
    {
    case Client::CGuardHit_Player::DEFAULT:
        m_fStateChangableTime = 0.3f;
        m_iAnimIndex = 7;
        fSpeed = 11.f;
        break;
    case Client::CGuardHit_Player::GUARDBREAK:
        m_fStateChangableTime = 0.5f;
        m_iAnimIndex = 6;
        fSpeed = 15.f;
        break;
    case Client::CGuardHit_Player::GUARD_END:
        break;
    default:
        break;
    }
    pOwner->Get_PhysicsCom()->Set_MaxSpeed(fSpeed);
    pOwner->Get_PhysicsCom()->Set_Dir(m_vLook * -1.f);
    pOwner->Get_PhysicsCom()->Set_Speed(fSpeed);

    pOwner->Get_Transform()->Set_LerpLook(m_vLook, 0.3f);

    __super::Enter(pOwner, pAnimator);
}

STATE_TYPE CGuardHit_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
    {
        return STATE_GUARD_PLAYER;
    }

    return __super::Tick(pOwner, pAnimator);
}

void CGuardHit_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
   CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_DEFAULT);
}

STATE_TYPE CGuardHit_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{


    return STATE_END;
}
