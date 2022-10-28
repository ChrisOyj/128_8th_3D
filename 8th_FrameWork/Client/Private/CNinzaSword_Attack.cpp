#include "stdafx.h"
#include "CNinzaSword_Attack.h"

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
#include "CHit_Air.h"

#include "CUnit_Player.h"

#include "CEffects_Factory.h"

#include "Functor.h"

CNinzaSword_Attack::CNinzaSword_Attack()
{
}

CNinzaSword_Attack::~CNinzaSword_Attack()
{
}

void CNinzaSword_Attack::OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos)
{
    if (!m_bOnce)
    {
        if (eColType == COL_ENEMYHITBOX)
        {
            CFunctor::Play_Sound(L"Eff_Hit_Kunai", CHANNEL_EFFECTS, PLAYER->Get_Transform()->Get_World(WORLD_POS));
            CEffects_Factory::Get_Instance()->Create_Effects_Turn(Convert_ToHash(L"Slash_0"), PLAYER, pOtherObj->Get_Transform()->Get_World(WORLD_POS));

            m_bOnce = true;
        }
            
    }
}

HRESULT CNinzaSword_Attack::Initialize()
{
    m_fTargetFindMaxDistance = 4.f;
    m_fColliderDisableTime = 0.2f;
    m_strRefBoneName = "R_Hand_Weapon_cnt_tr";
    //m_strRefBoneName = "L_Hand_Weapon_cnt_tr";
    m_strOriginRefBoneName = "Center_Position_Sword";
    m_fDelayTime = 0.3f;
    m_iSecondAttackExecuteFrame = 500;
    m_fDamage = 3.f;
    return S_OK;
}

void CNinzaSword_Attack::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->Switch_Bone(MODEL_PART_RWEAPON, m_strRefBoneName);

    m_fAttackTimeAcc = 0.f;
    m_bDisableCollider = false;
    m_bBoneChanged = false;
    m_bSecondDisableCol = false;
    m_bSecondExcuted = false;

    __super::Enter(pOwner, pAnimator);

}

STATE_TYPE CNinzaSword_Attack::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    //타겟이 있으면 타겟쪽으로 가야함
    _uint iCurAnimFrame = pAnimator->Get_CurAnimFrame();

    if (iCurAnimFrame >= m_iBoneChangeFrame)
    {
        if (!m_bBoneChanged)
        {
            m_bBoneChanged = true;
            pOwner->Switch_Bone(MODEL_PART_RWEAPON, m_strOriginRefBoneName);
        }
        
    }
    else if (iCurAnimFrame >= m_iSecondAttackExecuteFrame)
    {
        if (!m_bSecondExcuted)
        {
            m_bSecondExcuted = true;
            m_fAttackTimeAcc = 0.f;
            OnExecute(pOwner, pAnimator);
        }
        else
        {
            m_fAttackTimeAcc += fDT;
            if (m_fAttackTimeAcc >= m_fColliderDisableTime)
            {
                if (!m_bSecondDisableCol)
                    OnDisableCollider(pOwner, pAnimator);

                m_bSecondDisableCol = true;
            }
        }
    }
    else if (iCurAnimFrame < m_iAttackExecuteFrame)
    {
        CUnit* pTargetUnit = pOwner->Get_TargetUnit();
        if (pTargetUnit)
        {
            _float4 vMyPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
            _float4 vTargetPos = pTargetUnit->Get_Transform()->Get_World(WORLD_POS);

            _float4 vDir = (vTargetPos - vMyPos);

            if (vDir.Length() < m_fTargetFindMaxDistance)
            {
                vDir.y = 0.f;
                _float4 vGoalPos = vTargetPos + vDir.Normalize() * -0.3f;

                vDir = (vGoalPos - vMyPos);
                vMyPos += vDir * 4.f * fDT;

                pOwner->Get_Transform()->Set_World(WORLD_POS, vMyPos);
                pOwner->Get_Transform()->Make_WorldMatrix();
            }
        }
    }
    else 
    {
        if (!m_bExecuted) 
        {
            OnExecute(pOwner, pAnimator);
            m_bExecuted = true;
        }

        TickExecute(pOwner, pAnimator);
    }


    STATE_TYPE eType = STATE_END;


    if (iCurAnimFrame < m_iStateChangableFrame)
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

void CNinzaSword_Attack::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->Switch_Bone(MODEL_PART_RWEAPON, m_strOriginRefBoneName);
    static_cast<CUnit_Player*>(pOwner)->TurnOff_TrailBuffer();

    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CNinzaSword_Attack::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

void CNinzaSword_Attack::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::OnExecute(pOwner, pAnimator);
    static_cast<CUnit_Player*>(pOwner)->TurnOn_TrailBuffer();

}

void CNinzaSword_Attack::OnDisableCollider(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::OnDisableCollider(pOwner, pAnimator);
    static_cast<CUnit_Player*>(pOwner)->TurnOff_TrailBuffer();

}
