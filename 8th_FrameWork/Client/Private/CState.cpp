#include "stdafx.h"
#include "CState.h"

#include "CAnimator.h"
#include "CState_Manager.h"

#include "GameInstance.h"
#include "CUser.h"
#include "Functor.h"
#include "Transform.h"
#include "CUnit.h"
CState::CState()
{
}

CState::~CState()
{
}

void CState::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    CUser::Get_Instance()->Clear_KeyCommands();
    m_fTimeAcc = 0.f;
    pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
    pAnimator->Set_InterpolationTime(m_eAnimType, m_iAnimIndex, m_fInterPolationTime);
    pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, m_fAnimSpeed);
    m_bExecuted = false;
    
}

STATE_TYPE CState::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (!m_wstrSoundKey.empty())
    {
        m_fLoopSoundAcc += fDT;

        if (m_fLoopSoundAcc > m_fSoundLoopTime)
        {
            m_fLoopSoundAcc = 0.f;
            CFunctor::Play_Sound(m_wstrSoundKey, m_eChannel, pOwner->Get_Transform()->Get_World(WORLD_POS));

        }
    }
   



    m_fTimeAcc += fDT;

    STATE_TYPE eType = STATE_END;

    if (m_fTimeAcc >= m_fDelayTime)
    {
        if (!m_bExecuted)
        {
            OnExecute(pOwner, pAnimator);
            m_bExecuted = true;
        }

        TickExecute(pOwner, pAnimator);
    }

    if (m_fTimeAcc <= m_fStateChangableTime)
        return eType;

    for (auto& elem : m_vecAdjState)
    {
        if (!CState_Manager::Get_Instance()->Get_State(elem)->Is_CoolTimeOn())
            continue;

        eType = CState_Manager::Get_Instance()->Get_State(elem)->Check_Condition(pOwner, pAnimator);

        if (eType != STATE_END)
            break;
    }


    return eType;
}
