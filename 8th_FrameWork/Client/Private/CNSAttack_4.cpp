#include "stdafx.h"
#include "CNSAttack_4.h"

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

CNSAttack_4::CNSAttack_4()
{
}

CNSAttack_4::~CNSAttack_4()
{
}

CNSAttack_4* CNSAttack_4::Create()
{
    CNSAttack_4* pInstance = new CNSAttack_4();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CNSAttack_4");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

void CNSAttack_4::OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos)
{
    if (eColType == COL_ENEMYHITBOX)
    {
        if (m_bSecondExcuted)
            CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_FINISHATTACK);
    }
    __super::OnCollisionEnter(pOtherObj, eColType, vColPos);

}

HRESULT CNSAttack_4::Initialize()
{
    __super::Initialize();

    m_eAnimType = ANIM_ATTACK_NINZASWORD;
    m_iAnimIndex = 39;
    m_eStateType = STATE_NSATTACK_4_PLAYER;

    m_vecAdjState.push_back(STATE_IDLE_PLAYER);
    m_vecAdjState.push_back(STATE_DASH_PLAYER);
    m_vecAdjState.push_back(STATE_JUMP_PLAYER);
    m_vecAdjState.push_back(STATE_RASENSURIKEN_PLAYER);
    m_vecAdjState.push_back(STATE_RASENGAN);
    m_vecAdjState.push_back(STATE_KAMUI_PLAYER);
    m_vecAdjState.push_back(STATE_EIGHTGATES);
    m_vecAdjState.push_back(STATE_CHIDORI);
    m_vecAdjState.push_back(STATE_FIREBALL_PLAYER);
    m_vecAdjState.push_back(STATE_THROW_PLAYER);

    m_fAnimSpeed = 2.5f;
    m_fInterPolationTime = 0.05f;

    m_iBoneChangeFrame = 79;
    m_iAttackExecuteFrame = 13;
    m_iSecondAttackExecuteFrame = 35;
    m_iStateChangableFrame = 60;

   


    return S_OK;
}

void CNSAttack_4::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    /* For Attack */
    m_fSpeed = 13.f;
    m_fJumpPower = 0.f;
    m_fKnockbackPower = 7.f;
    m_eHitStateType = STATE_HIT_GROUND_ENEMY;
    m_iHitType = CHit_Ground::RIGHT;
    m_iColliderType = CUnit::LEFTFOOT;

    __super::Enter(pOwner, pAnimator);
}

STATE_TYPE CNSAttack_4::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CNSAttack_4::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CNSAttack_4::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    if (CUser::Get_Instance()->Get_LastKey() == KEY::LBUTTON &&
        pOwner->Is_NinzaSword()
        )
    {
        return m_eStateType;
    }


    return STATE_END;
}

void CNSAttack_4::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{

    

    if (m_bSecondExcuted)
    {
        CFunctor::Play_Sound(L"Eff_MoveStrong", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
        CFunctor::Play_Sound(L"Voice_Man_StrongAttack", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));
        //두번째 공격
         /* For Attack */
        m_fSpeed = 14.f;
        m_fJumpPower = 5.f;
        m_fKnockbackPower = 12.f;
        m_eHitStateType = STATE_BLASTED_GROUND_ENEMY;
        m_iHitType = CHit_Ground::LEFT;
        m_iColliderType = CUnit::NINZASWORD;
    }
    else
        CFunctor::Play_Sound(L"Eff_Move", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));


    __super::OnExecute(pOwner, pAnimator);
}
