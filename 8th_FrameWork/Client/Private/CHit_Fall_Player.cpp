#include "stdafx.h"
#include "CHit_Fall_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CScript_FollowCam.h"

#include "Transform.h"
#include "Physics.h"

#include "CState_Manager.h"
#include "CDown_Player.h"
#include "CGetUp_Player.h"

#include "Functor.h"

CHit_Fall_Player::CHit_Fall_Player()
{
}

CHit_Fall_Player::~CHit_Fall_Player()
{
}

CHit_Fall_Player* CHit_Fall_Player::Create()
{
    CHit_Fall_Player* pInstance = new CHit_Fall_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CHit_Fall_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

void CHit_Fall_Player::On_HitFall(HIT_TYPE eType, _float4 vLook, _float4 vDir, _float fSpeed, _float fGravityPower, _float fDamage)
{
    m_eHitType = eType;
    m_vLook = vLook;
    m_vDir = vDir;
    m_fSpeed = fSpeed;
    m_fGravityPower = fGravityPower;
    m_fDamage = fDamage;
}

HRESULT CHit_Fall_Player::Initialize()
{
    m_eAnimType = ANIM_BEATEN;
    m_iAnimIndex = 48;
    m_eStateType = STATE_HIT_FALL_PLAYER;
    m_fInterPolationTime = 0.05f;

    m_vecAdjState.push_back(STATE_DOWN_PLAYER);


    return S_OK;
}

void CHit_Fall_Player::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    //CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_HITHARD);
    CFunctor::Play_Sound(L"Eff_Hit_Hand", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));


    switch (m_eHitType)
    {
    case Client::CHit_Fall_Player::BACK:
        m_iAnimIndex = 48;
        break;
    case Client::CHit_Fall_Player::FRONT:
        m_iAnimIndex = 51;
        break;

    case Client::CHit_Fall_Player::HIT_END:
        break;
    default:
        break;
    }

    CTransform* pTransformCom = pOwner->Get_Transform();
    CPhysics* pPhysicsCom = pOwner->Get_PhysicsCom();

    pTransformCom->Set_LerpLook(m_vLook, 0.4f);
    pPhysicsCom->Set_Dir(m_vDir);
    pPhysicsCom->Set_MaxSpeed(m_fSpeed);
    pPhysicsCom->Set_Speed(m_fSpeed);
    if (!pPhysicsCom->Get_Physics().bAir)
        pPhysicsCom->Set_Jump(0.f);
    //pPhysicsCom->Get_Physics().fGravity = m_fGravityPower;



    __super::Enter(pOwner, pAnimator);
    pOwner->On_PlusHp(-m_fDamage);
}

STATE_TYPE CHit_Fall_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (CUser::Get_Instance()->Get_LastKey() == KEY::E &&
        CUser::Get_Instance()->Can_UseSkill(0))
    {
        if (!pOwner->Is_NightGuy())
            return STATE_DODGE_PLAYER;
    }
    return __super::Tick(pOwner, pAnimator);
}

void CHit_Fall_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
    static_cast<CDown_Player*>(CState_Manager::Get_Instance()->Get_State(STATE_DOWN_PLAYER))->Set_Front(
        (m_eHitType == FRONT) ? true : false
    );

    static_cast<CGetUp_Player*>(CState_Manager::Get_Instance()->Get_State(STATE_GETUP_PLAYER))->Set_Front(
        (m_eHitType == FRONT) ? true : false
    );

    pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 9.8f;
}

STATE_TYPE CHit_Fall_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return m_eStateType;

    return STATE_END;
}
