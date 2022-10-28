#include "stdafx.h"
#include "CHit_Air.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CScript_FollowCam.h"

#include "Transform.h"
#include "Physics.h"

#include "CEffects_Factory.h"

#include "Functor.h"
#include "CUnit_Player.h"
CHit_Air::CHit_Air()
{
}

CHit_Air::~CHit_Air()
{
}

CHit_Air* CHit_Air::Create()
{
    CHit_Air* pInstance = new CHit_Air();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CHit_Air");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

void CHit_Air::On_HitAir(HIT_TYPE eType, _float4 vLook, _float4 vDir, _float fSpeed, _float fJumpPower, _float fDamage)
{
    m_eHitType = eType;
    m_vLook = vLook;
    m_vDir = vDir;
    m_fSpeed = fSpeed;
    m_fJumpPower = fJumpPower;
    m_fDamage = fDamage;
}

HRESULT CHit_Air::Initialize()
{
    m_eAnimType = ANIM_BEATEN;
    m_iAnimIndex = 58;
    m_eStateType = STATE_HIT_AIR_ENEMY;
    m_fInterPolationTime = 0.05f;

    m_vecAdjState.push_back(STATE_FALL_ENEMY);
    m_vecAdjState.push_back(STATE_LAND_ENEMY);
    m_vecAdjState.push_back(STATE_WALLHIT_ENEMY);


    return S_OK;
}

void CHit_Air::Enter(CUnit* pOwner, CAnimator* pAnimator)
{

    switch (m_eHitType)
    {
    case Client::CHit_Air::SHORT:
        m_iAnimIndex = 9;
        break;
    case Client::CHit_Air::LONG:
        m_iAnimIndex = 7;
        break;
    case Client::CHit_Air::GOUP:
        m_iAnimIndex = 61;
        break;

    case Client::CHit_Air::HIT_END:
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
    pPhysicsCom->Set_Jump(m_fJumpPower);



    __super::Enter(pOwner, pAnimator);
    pOwner->On_PlusHp(-m_fDamage);
}

void CHit_Air::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CHit_Air::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{


    return STATE_END;
}
