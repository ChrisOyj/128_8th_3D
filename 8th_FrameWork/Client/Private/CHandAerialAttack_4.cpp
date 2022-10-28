#include "stdafx.h"
#include "CHandAerialAttack_4.h"

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
#include "CEffects_Factory.h"
#include "CUnit_Player.h"

#include "Functor.h"

CHandAerialAttack_4::CHandAerialAttack_4()
{
}

CHandAerialAttack_4::~CHandAerialAttack_4()
{
}

CHandAerialAttack_4* CHandAerialAttack_4::Create()
{
    CHandAerialAttack_4* pInstance = new CHandAerialAttack_4();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CHandAerialAttack_4");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

void CHandAerialAttack_4::OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos)
{
    if (eColType == COL_ENEMYHITBOX)
    {
        CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_FINISHATTACKDOWN);
        CEffects_Factory::Get_Instance()->Create_ScreenEffects(pOtherObj);
        CFunctor::Play_Sound(L"Eff_Hit_Hand", CHANNEL_EFFECTS, PLAYER->Get_Transform()->Get_World(WORLD_POS));

    }

}

HRESULT CHandAerialAttack_4::Initialize()
{
    m_eAnimType = ANIM_ATTACK_HAND;
    m_iAnimIndex = 0;
    m_eStateType = STATE_HANDAERIALATTACK_4_PLAYER;

    m_vecAdjState.push_back(STATE_FALL_PLAYER);
    m_vecAdjState.push_back(STATE_LAND_PLAYER);
    m_vecAdjState.push_back(STATE_DOUBLEJUMP_PLAYER);
    m_vecAdjState.push_back(STATE_AIRDASHSTART_PLAYER);
    m_vecAdjState.push_back(STATE_RASENSURIKEN_PLAYER);
    m_vecAdjState.push_back(STATE_CHIDORI);
    m_vecAdjState.push_back(STATE_FIREBALL_PLAYER);
    m_vecAdjState.push_back(STATE_THROW_PLAYER);

    m_fAnimSpeed = 1.1f;
    m_fInterPolationTime = 0.15f;
    m_fStateChangableTime = 0.8f;
    m_fDelayTime = 0.5f;

    /* For Attack */
    m_fDamage = 10.f;
    m_fSpeed = 0.f;
    m_fColliderDisableTime = 0.2f;
    m_fJumpPower = 0.f;
    m_fKnockbackPower = 8.f;
    m_eHitStateType = STATE_HIT_FALL_ENEMY;
    m_iHitType = CHit_Fall::BACK;
    m_iColliderType = CUnit::RIGHTFOOT;

    return S_OK;
}

void CHandAerialAttack_4::Enter(CUnit* pOwner, CAnimator* pAnimator)
{

    __super::Enter(pOwner, pAnimator);


    pOwner->Get_PhysicsCom()->Set_Jump(0.f);
    pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 0.f;
    pOwner->Get_PhysicsCom()->Set_Speed(8.f);
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 5.f;
}

STATE_TYPE CHandAerialAttack_4::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CHandAerialAttack_4::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
    pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 9.8f;
    CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_DEFAULT);

}

STATE_TYPE CHandAerialAttack_4::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    if (CUser::Get_Instance()->Get_LastKey() == KEY::RBUTTON)
    {
        return m_eStateType;
    }


    return STATE_END;
}

void CHandAerialAttack_4::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{

    CFunctor::Play_Sound(L"Eff_MoveStrong", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
    CFunctor::Play_Sound(L"Voice_Man_StrongAttack", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));

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
