#include "stdafx.h"
#include "CGuardHit_Enemy.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "Physics.h"
#include "Transform.h"
#include "CScript_FollowCam.h"
#include "CGuard.h"
#include "CEffects_Factory.h"
#include "CNavigation.h"

#include "Functor.h"

CGuardHit_Enemy::CGuardHit_Enemy()
{
}

CGuardHit_Enemy::~CGuardHit_Enemy()
{
}

CGuardHit_Enemy* CGuardHit_Enemy::Create()
{
    CGuardHit_Enemy* pInstance = new CGuardHit_Enemy();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CGuardHit_Enemy");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

void CGuardHit_Enemy::On_GuardHit(_float4 vLook, TYPE eType)
{
    m_vLook = vLook;
    m_eType = eType;
}

HRESULT CGuardHit_Enemy::Initialize()
{
    m_eAnimType = ANIM_GUARD;
    m_iAnimIndex = 7;

    m_eStateType = STATE_GUARDHIT_ENEMY;
    m_fInterPolationTime = 0.05f;
    m_fStateChangableTime = 0.4f;

    m_vecAdjState.push_back(STATE_RUN_ENEMY);
    m_vecAdjState.push_back(STATE_JUMP_ENEMY);
    m_vecAdjState.push_back(STATE_THROW_ENEMY);

    m_vecAdjState.push_back(STATE_GUARD_ENEMY);
    m_vecAdjState.push_back(STATE_WALK_ENEMY);
    m_vecAdjState.push_back(STATE_DASH_ENEMY);
    m_vecAdjState.push_back(STATE_IDLE_ENEMY);
    m_vecAdjState.push_back(STATE_HANDATTACK_1_ENEMY);


    return S_OK;
}

void CGuardHit_Enemy::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    CFunctor::Play_Sound(L"Eff_Hit_Guard", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));


    CEffects_Factory::Get_Instance()->Create_Effects(HASHCODE(CGuard), pOwner);
    //CEffects_Factory::Get_Instance()->Create_LandingEffects(pOwner->Get_Transform()->Get_World(WORLD_POS), GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());
    CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"GuardHit_0"), pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));


    CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_BACKDASH);
    _float fSpeed;
    switch (m_eType)
    {
    case Client::CGuardHit_Enemy::DEFAULT:
        m_fStateChangableTime = 0.4f;
        m_iAnimIndex = 7;
        fSpeed = 11.f;
        break;
    case Client::CGuardHit_Enemy::GUARDBREAK:
        m_fStateChangableTime = 0.5f;
        m_iAnimIndex = 6;
        fSpeed = 15.f;
        break;
    case Client::CGuardHit_Enemy::GUARD_END:
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

STATE_TYPE CGuardHit_Enemy::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
    {
        return STATE_GUARD_PLAYER;
    }

    return __super::Tick(pOwner, pAnimator);
}

void CGuardHit_Enemy::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_DEFAULT);
}

STATE_TYPE CGuardHit_Enemy::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{


    return STATE_END;
}
