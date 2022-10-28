#include "stdafx.h"
#include "CHandAerialAttack_4_Enemy.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "CCamera_Free.h"
#include "Physics.h"

#include "CUser.h"

#include "CScript_FollowCam.h"

#include "CState_Manager.h"
#include "CHit_Air.h"
#include "CHit_Fall.h"
#include "Functor.h"

CHandAerialAttack_4_Enemy::CHandAerialAttack_4_Enemy()
{
}

CHandAerialAttack_4_Enemy::~CHandAerialAttack_4_Enemy()
{
}

CHandAerialAttack_4_Enemy* CHandAerialAttack_4_Enemy::Create()
{
    CHandAerialAttack_4_Enemy* pInstance = new CHandAerialAttack_4_Enemy();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CHandAerialAttack_4_Enemy");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

void CHandAerialAttack_4_Enemy::OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos)
{
    if (eColType == COL_ENEMYHITBOX)
    {
        CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_FINISHATTACKDOWN);

    }

}

HRESULT CHandAerialAttack_4_Enemy::Initialize()
{
    m_eAnimType = ANIM_ATTACK_HAND;
    m_iAnimIndex = 0;
    m_eStateType = STATE_HANDAERIALATTACK_4_ENEMY;

    m_vecAdjState.push_back(STATE_FALL_ENEMY);
    m_vecAdjState.push_back(STATE_LAND_ENEMY);

    m_fAnimSpeed = 1.1f;
    m_fInterPolationTime = 0.15f;
    m_fStateChangableTime = 0.8f;
    m_fDelayTime = 0.5f;

    /* For Attack */
    m_fSpeed = 0.f;
    m_fColliderDisableTime = 0.2f;
    m_fJumpPower = 0.f;
    m_fKnockbackPower = 8.f;
    m_eHitStateType = STATE_HIT_FALL_PLAYER;
    m_iHitType = CHit_Fall::BACK;
    m_iColliderType = CUnit::RIGHTFOOT;

    return S_OK;
}

void CHandAerialAttack_4_Enemy::Enter(CUnit* pOwner, CAnimator* pAnimator)
{

    __super::Enter(pOwner, pAnimator);


    pOwner->Get_PhysicsCom()->Set_Jump(0.f);
    pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 0.f;
    pOwner->Get_PhysicsCom()->Set_Speed(8.f);
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 5.f;
}

STATE_TYPE CHandAerialAttack_4_Enemy::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CHandAerialAttack_4_Enemy::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
    pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 9.8f;

}

STATE_TYPE CHandAerialAttack_4_Enemy::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
   
    _float4 vCurPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
    _float4 vTargetPos = pOwner->Get_TargetUnit()->Get_Transform()->Get_World(WORLD_POS);
    vTargetPos.y = 0.f;
    vCurPos.y = 0.f;
    _float4 vDir = vTargetPos - vCurPos;
    _float fTargetLength = vDir.Length();


    if (fTargetLength < 4.f)
    {
            return m_eStateType;
    }


    return STATE_END;
}

void CHandAerialAttack_4_Enemy::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
    CFunctor::Play_Sound(L"Eff_Move", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
    pOwner->Get_PhysicsCom()->Set_Jump(5.f);
    pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 9.8f;


    _float4 vLook = pOwner->Get_Transform()->Get_World(WORLD_LOOK);

    pOwner->Enable_Collider(CUnit::COLLIDER_PARTS(m_iColliderType), true);
    pOwner->Get_PhysicsCom()->Set_Dir(vLook);
    pOwner->Get_PhysicsCom()->Set_MaxSpeed(m_fSpeed);
    pOwner->Get_PhysicsCom()->Set_Speed(m_fSpeed);


    _float4 vDown = _float4(0.f, -2.f, 0.f);
    vDown += vLook;
    vDown.Normalize();

    static_cast<CHit_Fall*>(CState_Manager::Get_Instance()->Get_State(m_eHitStateType))->On_HitFall(
        CHit_Fall::HIT_TYPE(m_iHitType), vLook * -1.f, vDown, m_fKnockbackPower, 20.f, m_fDamage);



}
