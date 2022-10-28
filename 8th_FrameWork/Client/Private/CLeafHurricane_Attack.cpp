#include "stdafx.h"
#include "CLeafHurricane_Attack.h"

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
CLeafHurricane_Attack::CLeafHurricane_Attack()
{
}

CLeafHurricane_Attack::~CLeafHurricane_Attack()
{
}

CLeafHurricane_Attack* CLeafHurricane_Attack::Create()
{
    CLeafHurricane_Attack* pInstance = new CLeafHurricane_Attack();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CLeafHurricane_Attack");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

void CLeafHurricane_Attack::OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos)
{
    if (eColType == COL_ENEMYHITBOX)
    {
        CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_FINISHATTACK);
    }

}

HRESULT CLeafHurricane_Attack::Initialize()
{
    m_eAnimType = ANIM_SKILL_ATTACK;
    m_iAnimIndex = 38;
    m_eStateType = STATE_LEAFHURRICANE_ATTACK_PLAYER;

    m_vecAdjState.push_back(STATE_IDLE_PLAYER);
    m_vecAdjState.push_back(STATE_DASH_PLAYER);
    m_vecAdjState.push_back(STATE_JUMP_PLAYER);

    m_fInterPolationTime = 0.05f;
    m_fStateChangableTime = 0.4f;
    m_fDelayTime = 0.05f;

    /* For Attack */
    m_fColliderDisableTime = 0.2f;
    m_fSpeed = 12.f;
    m_fJumpPower = 5.f;
    m_fKnockbackPower = 12.f;
    m_eHitStateType = STATE_BLASTED_GROUND_ENEMY;
    m_iHitType = CHit_Ground::HEAD;
    m_iColliderType = CUnit::LEFTFOOT;
    m_fDamage = 20.f;

    return S_OK;
}

void CLeafHurricane_Attack::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Enter(pOwner, pAnimator);

    if (pOwner->Get_PhysicsCom()->Get_Physics().bAir)
    {
        pOwner->Get_PhysicsCom()->Set_Jump(5.f);
        pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 9.8f;
    }
        
}

STATE_TYPE CLeafHurricane_Attack::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    if (m_fTimeAcc + fDT >= m_fStateChangableTime)
    {
        if (pOwner->Get_PhysicsCom()->Get_Physics().bAir)
            return STATE_FALL_PLAYER;
    }

    return __super::Tick(pOwner, pAnimator);
}

void CLeafHurricane_Attack::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);

    CAMERA_LERP(CAMERA_LERP_DEFAULT);
    CPhysics* pPhysicsCom = pOwner->Get_PhysicsCom();
    pPhysicsCom->Set_Jump(0.f);
    pPhysicsCom->Get_Physics().fGravity = 9.8f;
}

STATE_TYPE CLeafHurricane_Attack::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
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

void CLeafHurricane_Attack::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::OnExecute(pOwner, pAnimator);
    CEffects_Factory::Get_Instance()->Create_MultiEffects(L"LeafHurricane", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
    CUser::Get_Instance()->Get_FollowCam()->Start_ShakingCamera(0.07f, 0.3f);


}
