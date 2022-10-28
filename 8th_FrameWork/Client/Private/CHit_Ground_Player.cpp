#include "stdafx.h"
#include "CHit_Ground_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CNavigation.h"

#include "CScript_FollowCam.h"

#include "Transform.h"
#include "Physics.h"

#include "CState_Manager.h"

#include "CEffects_Factory.h"

#include "CHit_Fall_Player.h"
#include "CDown_Player.h"
#include "CGetUp_Player.h"

#include "Functor.h"

CHit_Ground_Player::CHit_Ground_Player()
{
}

CHit_Ground_Player::~CHit_Ground_Player()
{
}

CHit_Ground_Player* CHit_Ground_Player::Create()
{
    CHit_Ground_Player* pInstance = new CHit_Ground_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CHit_Ground_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
void CHit_Ground_Player::On_HitGround(HIT_TYPE eType, _float4 vLook, _float4 vDir, _float fSpeed, _float fJumpPower, _float fDamage)
{
    m_eHitType = eType;
    m_vLook = vLook;
    m_vDir = vDir;
    m_fSpeed = fSpeed;
    m_fJumpPower = fJumpPower;
    m_fDamage = fDamage;
}
void CHit_Ground_Player::On_HitGround(HIT_TYPE eType, _float fSpeed, _float fJumpPower, _float fDamage)
{
    m_eHitType = eType;
    m_fSpeed = fSpeed;
    m_fJumpPower = fJumpPower;
    m_fDamage = fDamage;
}
void CHit_Ground_Player::On_HitGround(_float4 vLook, _float4 vDir)
{
    m_vLook = vLook;
    m_vDir = vDir;
}
HRESULT CHit_Ground_Player::Initialize()
{
    m_eAnimType = ANIM_BEATEN;
    m_iAnimIndex = 58;
    m_eStateType = STATE_HIT_GROUND_PLAYER;
    m_fInterPolationTime = 0.05f;
    //m_fAnimSpeed = 1.5f;
    m_fStateChangableTime = 0.3f;

    m_vecAdjState.push_back(STATE_DASH_NIGHTGUY);

    m_vecAdjState.push_back(STATE_DASH_PLAYER);
    m_vecAdjState.push_back(STATE_GUARD_PLAYER);
    m_vecAdjState.push_back(STATE_THROW_PLAYER);


    return S_OK;
}

void CHit_Ground_Player::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_BACKDASH);

    if (m_eHitType < CHIDORIHIT)
    {
        CFunctor::Play_Sound(L"Eff_Hit_Hand", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
    }
    else
    {
        CFunctor::Play_Sound(L"Eff_KO_Hand", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
        CFunctor::Play_Sound(L"Voice_Man_Hit", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));

    }

    switch (m_eHitType)
    {
    case Client::CHit_Ground_Player::LEFT:
        m_iAnimIndex = 58;
        break;
    case Client::CHit_Ground_Player::RIGHT:
        m_iAnimIndex = 67;
        break;
    case Client::CHit_Ground_Player::HEAD:
        m_iAnimIndex = 59;
        break;
    case Client::CHit_Ground_Player::UPHEAD:
        m_iAnimIndex = 82;
        break;
    case Client::CHit_Ground_Player::UPSHOULDER:
        m_iAnimIndex = 83;
        break;

    case Client::CHit_Ground_Player::CHIDORIHIT:
        m_iAnimIndex = 62;
        m_fStateChangableTime = 1.2f;

        CFunctor::Play_Sound(L"Eff_Electric", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

        CEffects_Factory::Get_Instance()->Create_MultiEffects(L"ChidoriHitParticle", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));


        break;
    case Client::CHit_Ground_Player::HARDHIT:
        m_iAnimIndex = 62;

        m_fStateChangableTime = 1.2f;



        break;
    case Client::CHit_Ground_Player::SPINBLOW:
        m_iAnimIndex = 69;

        CEffects_Factory::Get_Instance()->Create_ScreenEffects(pOwner);
        m_vecAdjState.push_back(STATE_WALLHIT_PLAYER);
        m_fStateChangableTime = 20.f;


        break;
    case Client::CHit_Ground_Player::HIT_END:
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

    if (GET_COMPONENT_FROM(pOwner, CNavigation)->Is_OnWall())
    {
        _float4 vWallNormal = GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurWallNormal();
        if (0.8f < m_vLook.Dot(vWallNormal))
        {
            pPhysicsCom->Set_Speed(0.f);
        }
    }

    if (m_fJumpPower > 0.f)
        pPhysicsCom->Set_Jump(m_fJumpPower);


    __super::Enter(pOwner, pAnimator);

    pOwner->On_PlusHp(-m_fDamage);

}

STATE_TYPE CHit_Ground_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (!pOwner->Get_PhysicsCom()->Get_Physics().bAir)
    {
        //SpinBlow중에 땅에 닿으면
        if (m_eHitType == SPINBLOW)
        {
            static_cast<CDown_Player*>(CState_Manager::Get_Instance()->Get_State(STATE_DOWN_PLAYER))->Set_Front(
                false
            );

            return STATE_DOWN_PLAYER;
        }
    }




    if (pAnimator->Is_CurAnimFinished())
    {
        if (pOwner->Get_PhysicsCom()->Get_Physics().bAir)
        {
            //맞아서 떨어지는 상태로
            if (m_eHitType == SPINBLOW)
            {
                _float4 vLook = pOwner->Get_Transform()->Get_World(WORLD_LOOK);

                static_cast<CHit_Fall_Player*>(CState_Manager::Get_Instance()->Get_State(STATE_HIT_FALL_PLAYER))->On_HitFall(
                    CHit_Fall_Player::BACK, vLook * -1.f, vLook, 0.f, 0.f, 0.f);

                return STATE_HIT_FALL_PLAYER;
            }
            else
            {
                if (pOwner->Is_NightGuy())
                    return STATE_FALL_NIGHTGUY;
                else
                    return STATE_FALL_PLAYER;

            }

        }
        else
        {
            if (m_eHitType == HARDHIT || m_eHitType == CHIDORIHIT)
            {
                static_cast<CGetUp_Player*>(CState_Manager::Get_Instance()->Get_State(STATE_GETUP_PLAYER))->Set_Front(
                    true
                );

                return STATE_GETUP_PLAYER;

            }

            if (pOwner->Is_NightGuy())
                return STATE_IDLE_NIGHTGUY;
            else
                return STATE_IDLE_PLAYER;

        }

    }


    if (CUser::Get_Instance()->Get_LastKey() == KEY::E &&
        CUser::Get_Instance()->Can_UseSkill(0))
    {
        if (!pOwner->Is_NightGuy())
            return STATE_DODGE_PLAYER;
    }

    return __super::Tick(pOwner, pAnimator);
}

void CHit_Ground_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
    CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_DEFAULT);

}

STATE_TYPE CHit_Ground_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{


    return STATE_END;
}
