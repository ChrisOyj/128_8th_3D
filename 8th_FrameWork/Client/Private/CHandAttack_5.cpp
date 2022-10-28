#include "stdafx.h"
#include "CHandAttack_5.h"

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
#include "CBlasted_Ground.h"

#include "Functor.h"

CHandAttack_5::CHandAttack_5()
{
}

CHandAttack_5::~CHandAttack_5()
{
}

CHandAttack_5* CHandAttack_5::Create()
{
    CHandAttack_5* pInstance = new CHandAttack_5();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CHandAttack_5");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

void CHandAttack_5::OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos)
{
    //__super::OnCollisionEnter(pOtherObj, eColType, vColPos);

    if (eColType == COL_ENEMYHITBOX)
    {
        CFunctor::Play_Sound(L"Eff_KO_Hand", CHANNEL_EFFECTS, pOtherObj->Get_Transform()->Get_World(WORLD_POS));

        m_bColHit = true;
        m_fMyTimeAcc = 0.f;
        CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_FINISHATTACK);
    }
   
}

HRESULT CHandAttack_5::Initialize()
{
    m_eAnimType = ANIM_ATTACK_HAND;
    m_iAnimIndex = 62;
    m_eStateType = STATE_HANDATTACK_5_PLAYER;

    m_vecAdjState.push_back(STATE_IDLE_PLAYER);
    m_vecAdjState.push_back(STATE_DASH_PLAYER);
    m_vecAdjState.push_back(STATE_JUMP_PLAYER);
    m_vecAdjState.push_back(STATE_RASENSURIKEN_PLAYER);
    m_vecAdjState.push_back(STATE_KAMUI_PLAYER);
    m_vecAdjState.push_back(STATE_RASENGAN);
    m_vecAdjState.push_back(STATE_EIGHTGATES);
    m_vecAdjState.push_back(STATE_CHIDORI);
    m_vecAdjState.push_back(STATE_FIREBALL_PLAYER);
    m_vecAdjState.push_back(STATE_THROW_PLAYER);

    m_fInterPolationTime = 0.1f;
    m_fStateChangableTime = 0.7f;
    m_fDelayTime = 0.35f;

    /* For Attack */
    m_fSpeed = 11.f;
    m_fJumpPower = 5.f;
    m_fKnockbackPower = 12.f;
    m_eHitStateType = STATE_BLASTED_GROUND_ENEMY;
    m_iHitType = CHit_Ground::HEAD;
    m_iColliderType = CUnit::LEFTFOOT;
    m_fDamage = 10.f;

    return S_OK;
}

void CHandAttack_5::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Enter(pOwner, pAnimator);

}

STATE_TYPE CHandAttack_5::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_bColHit)
    {
        m_fMyTimeAcc += RealfDT;
        if (m_fMyTimeAcc > 0.4f)
        {
            CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_QUICKDEFAULT);
            m_bColHit = false;

        }
    }

    return __super::Tick(pOwner, pAnimator);
}

void CHandAttack_5::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);

}

STATE_TYPE CHandAttack_5::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Dash으로 오는 조건
    1. 키눌렸을때
    스킬
    스킬 쿨타임도 확인하고
    스킬 챠크라 게이지도 확인하고

    */
    if (CUser::Get_Instance()->Get_LastKey() == KEY::LBUTTON)
    {
        return m_eStateType;
    }


    return STATE_END;
}

void CHandAttack_5::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{

    CFunctor::Play_Sound(L"Eff_MoveStrong", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
    CFunctor::Play_Sound(L"Voice_Man_StrongAttack", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));
    __super::OnExecute(pOwner, pAnimator);

}
