#include "stdafx.h"
#include "CHandAttack_1.h"

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


#include "CEffects_Factory.h"
#include "CCell.h"
#include "CNavigation.h"
#include "CWoodHand.h"
#include "CWoodHand_R.h"
#include "CWoodWall.h"
#include "CBurger.h"

#include "Functor.h"

CHandAttack_1::CHandAttack_1()
{
}

CHandAttack_1::~CHandAttack_1()
{
}

CHandAttack_1* CHandAttack_1::Create()
{
    CHandAttack_1* pInstance = new CHandAttack_1();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CHandAttack_1");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}


HRESULT CHandAttack_1::Initialize()
{
    m_eAnimType = ANIM_ATTACK_HAND;
    m_iAnimIndex = 40;
    m_eStateType = STATE_HANDATTACK_1_PLAYER;

    m_vecAdjState.push_back(STATE_IDLE_PLAYER);
    m_vecAdjState.push_back(STATE_DASH_PLAYER);
    m_vecAdjState.push_back(STATE_NSATTACK_2_PLAYER);
    m_vecAdjState.push_back(STATE_HANDATTACK_2_PLAYER);
    m_vecAdjState.push_back(STATE_JUMP_PLAYER);
    m_vecAdjState.push_back(STATE_HANDATTACK_POWER_PLAYER);
    m_vecAdjState.push_back(STATE_RASENSURIKEN_PLAYER);
    m_vecAdjState.push_back(STATE_RASENGAN);
    m_vecAdjState.push_back(STATE_KAMUI_PLAYER);
    m_vecAdjState.push_back(STATE_EIGHTGATES);
    m_vecAdjState.push_back(STATE_CHIDORI);
    m_vecAdjState.push_back(STATE_FIREBALL_PLAYER);
    m_vecAdjState.push_back(STATE_THROW_PLAYER);

    m_fAnimSpeed = 1.4f;
    m_fInterPolationTime = 0.05f;
    m_fStateChangableTime = 0.4f;
    m_fDelayTime = 0.3f;

    /* For Attack */
    m_fSpeed = 7.f;
    m_fJumpPower = 0.f;
    m_fKnockbackPower = 5.f;
    m_eHitStateType = STATE_HIT_GROUND_ENEMY;
    m_iHitType = CHit_Ground::RIGHT;
    m_iColliderType = CUnit::LEFTHAND;

    return S_OK;
}

void CHandAttack_1::Enter(CUnit* pOwner, CAnimator* pAnimator)
{

   

    CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_DEFAULT);

    __super::Enter(pOwner, pAnimator);
}

STATE_TYPE CHandAttack_1::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
  /*  if (KEY(H, TAP))
    {
        return STATE_STRUGGLE_ENEMY;
    }*/

    return __super::Tick(pOwner, pAnimator);
}

void CHandAttack_1::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    //CUser::Get_Instance()->On_BossDeath();

    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CHandAttack_1::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    if (CUser::Get_Instance()->Get_LastKey() == KEY::LBUTTON)
    {
        return m_eStateType;
    }


    return STATE_END;
}

void CHandAttack_1::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::OnExecute(pOwner, pAnimator);
    CFunctor::Play_Sound(L"Eff_Move", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
    CFunctor::Play_Sound(L"Voice_Man_Attack", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));

}
