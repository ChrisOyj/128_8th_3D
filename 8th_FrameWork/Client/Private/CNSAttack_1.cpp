#include "stdafx.h"
#include "CNSAttack_1.h"

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

#include "CUnit_Player.h"

#include "Functor.h"

CNSAttack_1::CNSAttack_1()
{
}

CNSAttack_1::~CNSAttack_1()
{
}

CNSAttack_1* CNSAttack_1::Create()
{
    CNSAttack_1* pInstance = new CNSAttack_1();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CNSAttack_1");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

void CNSAttack_1::OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos)
{
    __super::OnCollisionEnter(pOtherObj, eColType, vColPos);
}

HRESULT CNSAttack_1::Initialize()
{
    __super::Initialize();

    m_eAnimType = ANIM_ATTACK_NINZASWORD;
    //m_eAnimType = ANIM_ATTACK_BIGSWORD;
    m_iAnimIndex = 28;
    m_eStateType = STATE_NSATTACK_1_PLAYER;

    m_vecAdjState.push_back(STATE_IDLE_PLAYER);
    m_vecAdjState.push_back(STATE_DASH_PLAYER);
    m_vecAdjState.push_back(STATE_NSATTACK_2_PLAYER);
    m_vecAdjState.push_back(STATE_JUMP_PLAYER);
    m_vecAdjState.push_back(STATE_HANDATTACK_POWER_PLAYER);
    m_vecAdjState.push_back(STATE_HANDATTACK_2_PLAYER);
    m_vecAdjState.push_back(STATE_RASENSURIKEN_PLAYER);
    m_vecAdjState.push_back(STATE_RASENGAN);
    m_vecAdjState.push_back(STATE_KAMUI_PLAYER);
    m_vecAdjState.push_back(STATE_EIGHTGATES);
    m_vecAdjState.push_back(STATE_CHIDORI);
    m_vecAdjState.push_back(STATE_FIREBALL_PLAYER);
    m_vecAdjState.push_back(STATE_THROW_PLAYER);

    m_fAnimSpeed = 2.5f;
    m_fInterPolationTime = 0.05f;

    m_iBoneChangeFrame = 72;
    m_iAttackExecuteFrame = 17;
    m_iStateChangableFrame = 30;
    m_fDamage = 5.f;

    /* For Attack */
    m_fSpeed = 10.f;
    m_fJumpPower = 0.f;
    m_fKnockbackPower = 7.f;
    m_eHitStateType = STATE_HIT_GROUND_ENEMY;
    m_iHitType = CHit_Ground::LEFT;
    m_iColliderType = CUnit::NINZASWORD;


    return S_OK;
}

void CNSAttack_1::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_DEFAULT);

    __super::Enter(pOwner, pAnimator);
}

STATE_TYPE CNSAttack_1::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CNSAttack_1::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);

}

STATE_TYPE CNSAttack_1::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    if (CUser::Get_Instance()->Get_LastKey() == KEY::LBUTTON &&
        pOwner->Is_NinzaSword()
        )
    {
        return m_eStateType;
    }


    return STATE_END;
}

void CNSAttack_1::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{

    CFunctor::Play_Sound(L"Eff_Move", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
    CFunctor::Play_Sound(L"Voice_Man_Attack", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));
    __super::OnExecute(pOwner, pAnimator);
}
