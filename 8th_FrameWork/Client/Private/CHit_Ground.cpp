#include "stdafx.h"
#include "CHit_Ground.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CNavigation.h"

#include "CScript_FollowCam.h"

#include "Transform.h"
#include "Physics.h"

#include "CEffects_Factory.h"
#include "CState_Manager.h"
#include "CGetUp_Enemy.h"
#include "CHit_Fall.h"
#include "CDown_Enemy.h"

#include "CBossAura.h"
#include "CUnit_Player.h"
#include "Functor.h"

CHit_Ground::CHit_Ground()
{
}

CHit_Ground::~CHit_Ground()
{
}

CHit_Ground* CHit_Ground::Create()
{
    CHit_Ground* pInstance = new CHit_Ground();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CHit_Ground");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
void CHit_Ground::On_HitGround(HIT_TYPE eType, _float4 vLook, _float4 vDir, _float fSpeed, _float fJumpPower, _float fDamage)
{
    m_eHitType = eType;
    m_vLook = vLook;
    m_vDir = vDir;
    m_fSpeed = fSpeed;
    m_fJumpPower = fJumpPower;
    m_fDamage = fDamage;
}

void CHit_Ground::On_HitGround(HIT_TYPE eType, _float fSpeed, _float fJumpPower, _float fDamage)
{
    m_eHitType = eType;
    m_fSpeed = fSpeed;
    m_fJumpPower = fJumpPower;
    m_fDamage = fDamage;
}

void CHit_Ground::On_HitGround(_float4 vLook, _float4 vDir)
{
    m_vLook = vLook;
    m_vDir = vDir;
}

void CHit_Ground::On_HitGroundDefault(_float4 vLook, _float4 vDir)
{
    m_eHitType = HEAD;
    m_vLook = vLook;
    m_vDir = vDir;
    m_fSpeed = 7.f;
    m_fJumpPower = 0.f;
    m_fDamage = 3.f;
}

HRESULT CHit_Ground::Initialize()
{
    m_eAnimType = ANIM_BEATEN;
    m_iAnimIndex = 58;
    m_eStateType = STATE_HIT_GROUND_ENEMY;
    m_fInterPolationTime = 0.05f;

    return S_OK;
}

void CHit_Ground::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
   




    if (pOwner->Get_Status().fMaxHP > 200.f)
    {
        if (pOwner->Get_Status().fHP < pOwner->Get_Status().fMaxHP * 0.1f)
        {
            if (CUser::Get_Instance()->On_BossPhaseEnd())
            {
                CFunctor::Play_Sound(L"Voice_Boss_KotoStart", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));
                CEffects_Factory::Get_Instance()->Create_MultiEffects(L"BossAuraStart", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
            }
        }

        else if (pOwner->Get_Status().fHP < pOwner->Get_Status().fMaxHP * 0.4f)
        {
            if (CUser::Get_Instance()->On_BossPhase2())
                return;
        }

       
    }
  


    switch (m_eHitType)
    {
    case Client::CHit_Ground::LEFT:
        m_iAnimIndex = 58;
        break;
    case Client::CHit_Ground::RIGHT:
        m_iAnimIndex = 67;
        break;
    case Client::CHit_Ground::HEAD:
        m_iAnimIndex = 59;

        if (m_fSpeed <= 2.f)
            m_vDir *= -1.f;

        break;
    case Client::CHit_Ground::UPHEAD:
        m_iAnimIndex = 82;
        break;
    case Client::CHit_Ground::UPSHOULDER:
        m_iAnimIndex = 83;
        break;
    case Client::CHit_Ground::CHIDORIHIT:
        m_iAnimIndex = 62;


        CEffects_Factory::Get_Instance()->Create_MultiEffects(L"ChidoriHitParticle", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));


        break;
    case Client::CHit_Ground::HARDHIT:
        m_iAnimIndex = 62;




        break;
    case Client::CHit_Ground::SPINBLOW:
        m_iAnimIndex = 69;

        CEffects_Factory::Get_Instance()->Create_ScreenEffects(pOwner);
        m_vecAdjState.push_back(STATE_WALLHIT_ENEMY);



        break;
    case Client::CHit_Ground::HIT_END:
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

STATE_TYPE CHit_Ground::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (!pOwner->Get_PhysicsCom()->Get_Physics().bAir)
    {
        //SpinBlow중에 땅에 닿으면
        if (m_eHitType == SPINBLOW)
        {
            static_cast<CDown_Enemy*>(CState_Manager::Get_Instance()->Get_State(STATE_DOWN_ENEMY))->Set_Front(
                false
            );

            static_cast<CGetUp_Enemy*>(CState_Manager::Get_Instance()->Get_State(STATE_GETUP_ENEMY))->Set_Front(
                false
            );

            return STATE_DOWN_ENEMY;
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

                static_cast<CHit_Fall*>(CState_Manager::Get_Instance()->Get_State(STATE_HIT_FALL_ENEMY))->On_HitFall(
                    CHit_Fall::BACK, vLook * -1.f, vLook, 0.f, 0.f, 0.f);

                static_cast<CDown_Enemy*>(CState_Manager::Get_Instance()->Get_State(STATE_DOWN_ENEMY))->Set_Front(
                    false
                );

                static_cast<CGetUp_Enemy*>(CState_Manager::Get_Instance()->Get_State(STATE_GETUP_ENEMY))->Set_Front(
                    false
                );

                return STATE_HIT_FALL_ENEMY;
            }
            else
                return STATE_FALL_ENEMY;

        }
        else
        {
            if (m_eHitType == HARDHIT || m_eHitType == CHIDORIHIT)
            {
                static_cast<CGetUp_Enemy*>(CState_Manager::Get_Instance()->Get_State(STATE_GETUP_ENEMY))->Set_Front(
                    true
                );

                return STATE_GETUP_ENEMY;

            }

            return STATE_IDLE_ENEMY;

        }

    }



    return __super::Tick(pOwner, pAnimator);
}

void CHit_Ground::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CHit_Ground::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{


    return STATE_END;
}
