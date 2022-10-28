#include "stdafx.h"
#include "CHit_Fall.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CScript_FollowCam.h"

#include "Transform.h"
#include "Physics.h"

#include "CState_Manager.h"
#include "CDown_Enemy.h"

#include "CEffects_Factory.h"
#include "CScreenEffect.h"
#include "CGetUp_Enemy.h"

#include "Functor.h"
#include "CUnit_Player.h"

CHit_Fall::CHit_Fall()
{
}

CHit_Fall::~CHit_Fall()
{
}

CHit_Fall* CHit_Fall::Create()
{
    CHit_Fall* pInstance = new CHit_Fall();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CHit_Fall");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

void CHit_Fall::On_HitFall(HIT_TYPE eType, _float4 vLook, _float4 vDir, _float fSpeed, _float fGravityPower, _float fDamage)
{
    m_eHitType = eType;
    m_vLook = vLook;
    m_vDir = vDir;
    m_fSpeed = fSpeed;
    m_fGravityPower = fGravityPower;
    m_fDamage = fDamage;
}

HRESULT CHit_Fall::Initialize()
{
    m_eAnimType = ANIM_BEATEN;
    m_iAnimIndex = 48;
    m_eStateType = STATE_HIT_FALL_ENEMY;
    m_fInterPolationTime = 0.05f;

    m_vecAdjState.push_back(STATE_DOWN_ENEMY);


    return S_OK;
}

void CHit_Fall::Enter(CUnit* pOwner, CAnimator* pAnimator)
{

    switch (m_eHitType)
    {
    case Client::CHit_Fall::BACK:
        m_iAnimIndex = 48;
        break;
    case Client::CHit_Fall::FRONT:
        m_iAnimIndex = 51;
        break;

    case Client::CHit_Fall::HIT_END:
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

void CHit_Fall::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
    static_cast<CDown_Enemy*>(CState_Manager::Get_Instance()->Get_State(STATE_DOWN_ENEMY))->Set_Front(
        (m_eHitType == FRONT) ? true : false
    );

    static_cast<CGetUp_Enemy*>(CState_Manager::Get_Instance()->Get_State(STATE_GETUP_ENEMY))->Set_Front(
        (m_eHitType == FRONT) ? true : false
    );

    pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 9.8f;
}

STATE_TYPE CHit_Fall::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return m_eStateType;

    return STATE_END;
}
