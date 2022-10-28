#include "stdafx.h"
#include "CStruggle_Enemy.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CScript_FollowCam.h"

#include "Transform.h"
#include "Physics.h"

#include "CEffects_Factory.h"
#include "CScreenEffect.h"

#include "Functor.h"

CStruggle_Enemy::CStruggle_Enemy()
{
}

CStruggle_Enemy::~CStruggle_Enemy()
{
}

CStruggle_Enemy* CStruggle_Enemy::Create()
{
    CStruggle_Enemy* pInstance = new CStruggle_Enemy();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CStruggle_Enemy");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CStruggle_Enemy::Initialize()
{
    m_eAnimType = ANIM_BEATEN;
    m_iAnimIndex = 13;
    m_eStateType = STATE_STRUGGLE_ENEMY;
    m_fInterPolationTime = 0.1f;
    m_fAnimSpeed = 0.6f;
    m_fStateChangableTime = 6.f;

    m_vecAdjState.push_back(STATE_FALL_ENEMY);


    return S_OK;
}

void CStruggle_Enemy::Enter(CUnit* pOwner, CAnimator* pAnimator)
{

    pOwner->Get_PhysicsCom()->Set_Speed(0.f);
    pOwner->Get_PhysicsCom()->Set_Jump(0.f);
    pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 0.f;


    m_EffectsList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"WaterBall", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
   CEffects_Factory::Get_Instance()->Create_MultiEffects(L"WaterPrison_Start", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));

    __super::Enter(pOwner, pAnimator);
}

STATE_TYPE CStruggle_Enemy::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    m_fParticleAcc += fDT;
    if (m_fParticleAcc > m_fParticleCreateTime)
    {
        CEffects_Factory::Get_Instance()->Create_MultiEffects(L"WaterParticle", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
        m_fParticleAcc = 0.f;
    }

    return __super::Tick(pOwner, pAnimator);
}

void CStruggle_Enemy::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    CFunctor::Play_Sound(L"Eff_WaterOut", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

    /* 할거없음 */
    pOwner->Get_PhysicsCom()->Set_Jump(0.f);
    pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 9.8f;


    for (auto& elem : m_EffectsList)
        DISABLE_GAMEOBJECT(elem);
    CEffects_Factory::Get_Instance()->Create_MultiEffects(L"WaterPrison_Start", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
}

STATE_TYPE CStruggle_Enemy::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{


    return STATE_END;
}
