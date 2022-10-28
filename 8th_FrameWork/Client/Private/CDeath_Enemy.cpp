#include "stdafx.h"
#include "CDeath_Enemy.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CNavigation.h"

#include "CScript_FollowCam.h"

#include "Transform.h"
#include "Physics.h"

#include "CEffects_Factory.h"
#include "CScreenEffect.h"
#include "CUnit_Player.h"
#include "Functor.h"

CDeath_Enemy::CDeath_Enemy()
{
}

CDeath_Enemy::~CDeath_Enemy()
{
}

CDeath_Enemy* CDeath_Enemy::Create()
{
    CDeath_Enemy* pInstance = new CDeath_Enemy();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CDeath_Enemy");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CDeath_Enemy::Initialize()
{
    m_eAnimType = ANIM_BEATEN;
    m_iAnimIndex = 86;
    m_eStateType = STATE_DEATH_ENEMY;
    m_fInterPolationTime = 0.1f;

    return S_OK;
}

void CDeath_Enemy::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    if (PLAYER->Is_NinzaSword())
        CFunctor::Play_Sound(L"Eff_KO_Sword", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
    else
        CFunctor::Play_Sound(L"Eff_KO_Hand", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

    CEffects_Factory::Get_Instance()->Create_ScreenEffects(pOwner);

    CTransform* pTransformCom = pOwner->Get_Transform();
    CPhysics* pPhysicsCom = pOwner->Get_PhysicsCom();

    if (pPhysicsCom->Get_Physics().bAir)
    {
        m_iAnimIndex = 69;
        pPhysicsCom->Set_MaxSpeed(10.f);
        pPhysicsCom->Set_Speed(10.f);
        pPhysicsCom->Set_Jump(12.f);

    }
    else
    {
        m_iAnimIndex = 86;
        pPhysicsCom->Set_MaxSpeed(14.f);
        pPhysicsCom->Set_Speed(14.f);
    }

    if (pOwner->Get_Status().fMaxHP > 200.f)
    {
        m_bBoss = true;
    }

    __super::Enter(pOwner, pAnimator);
}

STATE_TYPE CDeath_Enemy::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_bBoss && !m_bBGM)
    {
        _float fBGMTime = 1.5f;
        if (m_fTimeAcc > fBGMTime)
        {
            m_bBGM = true;
            GAMEINSTANCE->Set_ChannelVolume((CHANNEL_GROUP)CHANNEL_BGM, 0.3f);
            GAMEINSTANCE->Stop_Sound((CHANNEL_GROUP)CHANNEL_BGM);
        }
        else
        {
            _float fVolume = 0.3f * ((fBGMTime - m_fTimeAcc) / fBGMTime);
            GAMEINSTANCE->Set_ChannelVolume((CHANNEL_GROUP)CHANNEL_BGM, fVolume);

        }


    }




    if (pAnimator->Is_CurAnimFinished())
    {
        CFunctor::Play_Sound(L"Eff_Smoke", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
        CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Smoke", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
        DISABLE_GAMEOBJECT(pOwner);
    }

    return __super::Tick(pOwner, pAnimator);
}

void CDeath_Enemy::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */

}

STATE_TYPE CDeath_Enemy::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{


    return STATE_END;
}
