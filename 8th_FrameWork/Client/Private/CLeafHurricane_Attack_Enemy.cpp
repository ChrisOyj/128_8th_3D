#include "stdafx.h"
#include "CLeafHurricane_Attack_Enemy.h"

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
#include "CEffects_Factory.h"
#include "CNavigation.h"

#include "Functor.h"

CLeafHurricane_Attack_Enemy::CLeafHurricane_Attack_Enemy()
{
}

CLeafHurricane_Attack_Enemy::~CLeafHurricane_Attack_Enemy()
{
}

CLeafHurricane_Attack_Enemy* CLeafHurricane_Attack_Enemy::Create()
{
    CLeafHurricane_Attack_Enemy* pInstance = new CLeafHurricane_Attack_Enemy();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CLeafHurricane_Attack_Enemy");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

void CLeafHurricane_Attack_Enemy::OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos)
{
    if (eColType == COL_ENEMYHITBOX)
    {
        CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_FINISHATTACK);
    }

}

HRESULT CLeafHurricane_Attack_Enemy::Initialize()
{
    m_eAnimType = ANIM_SKILL_ATTACK;
    m_iAnimIndex = 38;
    m_eStateType = STATE_LEAFHURRICANE_ATTACK_ENEMY;


    m_vecAdjState.push_back(STATE_RUN_ENEMY);
    m_vecAdjState.push_back(STATE_JUMP_ENEMY);


    m_vecAdjState.push_back(STATE_IDLE_ENEMY);
    m_vecAdjState.push_back(STATE_DASH_ENEMY);

    m_fInterPolationTime = 0.05f;
    m_fStateChangableTime = 0.4f;
    m_fDelayTime = 0.05f;

    /* For Attack */
    m_fColliderDisableTime = 0.2f;
    m_fSpeed = 12.f;
    m_fJumpPower = 5.f;
    m_fKnockbackPower = 12.f;
    m_eHitStateType = STATE_BLASTED_GROUND_PLAYER;
    m_iHitType = CHit_Ground::HEAD;
    m_iColliderType = CUnit::LEFTFOOT;
    m_fDamage = 20.f;

    return S_OK;
}

void CLeafHurricane_Attack_Enemy::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Enter(pOwner, pAnimator);
    CFunctor::Play_Sound(L"Voice_LeafHurricane", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));
    CFunctor::Play_Sound(L"Eff_RasenShuriken_Boom", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));


    if (pOwner->Get_PhysicsCom()->Get_Physics().bAir)
    {
        pOwner->Get_PhysicsCom()->Set_Jump(5.f);
        pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 9.8f;
    }

}

STATE_TYPE CLeafHurricane_Attack_Enemy::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    if (m_fTimeAcc + fDT >= m_fStateChangableTime)
    {
        if (pOwner->Get_PhysicsCom()->Get_Physics().bAir)
            return STATE_FALL_ENEMY;
    }

    return __super::Tick(pOwner, pAnimator);
}

void CLeafHurricane_Attack_Enemy::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);

    CPhysics* pPhysicsCom = pOwner->Get_PhysicsCom();
    pPhysicsCom->Set_Jump(0.f);
    pPhysicsCom->Get_Physics().fGravity = 9.8f;
}

STATE_TYPE CLeafHurricane_Attack_Enemy::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Dash으로 오는 조건
    1. 키눌렸을때
    스킬
    스킬 쿨타임도 확인하고
    스킬 챠크라 게이지도 확인하고

    */
    if (1)
    {
        return m_eStateType;
    }


    return STATE_END;
}

void CLeafHurricane_Attack_Enemy::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::OnExecute(pOwner, pAnimator);

    CEffects_Factory::Get_Instance()->Create_MultiEffects(L"LeafHurricane", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));


}
