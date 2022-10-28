#include "stdafx.h"
#include "CNSAerialAttack_2.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "CCamera_Free.h"
#include "Physics.h"

#include "CUser.h"

#include "CScript_FollowCam.h"

#include "CState_Manager.h"
#include "CHit_Ground.h"
#include "CHit_Air.h"
#include "CUnit_Player.h"
#include "Functor.h"

CNSAerialAttack_2::CNSAerialAttack_2()
{
}

CNSAerialAttack_2::~CNSAerialAttack_2()
{
}

CNSAerialAttack_2* CNSAerialAttack_2::Create()
{
    CNSAerialAttack_2* pInstance = new CNSAerialAttack_2();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CNSAerialAttack_2");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

void CNSAerialAttack_2::OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos)
{
    __super::OnCollisionEnter(pOtherObj, eColType, vColPos);
}

HRESULT CNSAerialAttack_2::Initialize()
{
    __super::Initialize();

    m_eAnimType = ANIM_ATTACK_NINZASWORD;
    m_iAnimIndex = 9;
    m_eStateType = STATE_NSAERIALATTACK_2_PLAYER;

    m_vecAdjState.push_back(STATE_FALL_PLAYER);
    m_vecAdjState.push_back(STATE_AIRDASHSTART_PLAYER);
    m_vecAdjState.push_back(STATE_HANDAERIALATTACK_3_PLAYER);
    m_vecAdjState.push_back(STATE_NSAERIALATTACK_3_PLAYER);
    m_vecAdjState.push_back(STATE_DOUBLEJUMP_PLAYER);
    m_vecAdjState.push_back(STATE_RASENSURIKEN_PLAYER);
    m_vecAdjState.push_back(STATE_CHIDORI);
    m_vecAdjState.push_back(STATE_FIREBALL_PLAYER);
    m_vecAdjState.push_back(STATE_THROW_PLAYER);
    m_vecAdjState.push_back(STATE_WATERPRISON_PLAYER);

    m_fAnimSpeed = 2.5f;
    m_fInterPolationTime = 0.05f;

    m_iBoneChangeFrame = 62;
    m_iAttackExecuteFrame = 14;
    m_iStateChangableFrame = 30;

    /* For Attack */
    m_fSpeed = 7.f;
    m_fColliderDisableTime = 0.1f;
    m_fJumpPower = 4.f;
    m_fKnockbackPower = 5.f;
    m_eHitStateType = STATE_HIT_AIR_ENEMY;
    m_iHitType = CHit_Air::SHORT;
    m_iColliderType = CUnit::NINZASWORD;


    return S_OK;
}

void CNSAerialAttack_2::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Enter(pOwner, pAnimator);

    pOwner->Get_PhysicsCom()->Set_Jump(0.f);
    pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 0.f;
    pOwner->Get_PhysicsCom()->Mul_Speed(0.5f);
}

STATE_TYPE CNSAerialAttack_2::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
   /* _uint iCurAnimFrame = pAnimator->Get_CurAnimFrame();

    if (iCurAnimFrame >= m_iStateChangableFrame)
    {
        if (pOwner->Get_PhysicsCom()->Get_Physics().fGravity < 9.8f)
        {
            pOwner->Get_PhysicsCom()->Set_Jump(0.f);
            pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 9.8f;
        }
    }*/
    return __super::Tick(pOwner, pAnimator);
}

void CNSAerialAttack_2::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);

    if (pOwner->Get_PhysicsCom()->Get_Physics().fGravity < 9.8f)
    {
        pOwner->Get_PhysicsCom()->Set_Jump(0.f);
        pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 9.8f;
    }

}

STATE_TYPE CNSAerialAttack_2::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    if (CUser::Get_Instance()->Get_LastKey() == KEY::LBUTTON &&
        pOwner->Is_NinzaSword()
        )
    {
        return m_eStateType;
    }


    return STATE_END;
}

void CNSAerialAttack_2::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
    CFunctor::Play_Sound(L"Eff_Move", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
    CFunctor::Play_Sound(L"Voice_Man_Attack", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));
    __super::OnExecute(pOwner, pAnimator);
}
