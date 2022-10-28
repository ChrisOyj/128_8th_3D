#include "stdafx.h"
#include "CBlasted_Ground.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CScript_FollowCam.h"

#include "Transform.h"
#include "Physics.h"

#include "CEffects_Factory.h"
#include "CScreenEffect.h"
CBlasted_Ground::CBlasted_Ground()
{
}

CBlasted_Ground::~CBlasted_Ground()
{
}

CBlasted_Ground* CBlasted_Ground::Create()
{
    CBlasted_Ground* pInstance = new CBlasted_Ground();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CBlasted_Ground");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
void CBlasted_Ground::On_BlastedGround(_float4 vLook, _float4 vDir, _float fSpeed, _float fJumpPower, _float fDamage)
{
    m_vLook = vLook;
    m_vDir = vDir;
    m_fSpeed = fSpeed;
    m_fJumpPower = fJumpPower;
    m_fDamage = fDamage;
}

HRESULT CBlasted_Ground::Initialize()
{
    m_eAnimType = ANIM_BEATEN;
    m_iAnimIndex = 25;
    m_eStateType = STATE_BLASTED_GROUND_ENEMY;
    m_fInterPolationTime = 0.05f;

    m_vecAdjState.push_back(STATE_WALLHIT_ENEMY);


    return S_OK;
}

void CBlasted_Ground::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    CEffects_Factory::Get_Instance()->Create_ScreenEffects(pOwner);

    CTransform* pTransformCom = pOwner->Get_Transform();
    CPhysics* pPhysicsCom = pOwner->Get_PhysicsCom();

    pTransformCom->Set_LerpLook(m_vLook, 0.3f);
    pPhysicsCom->Set_Dir(m_vDir);
    pPhysicsCom->Set_MaxSpeed(m_fSpeed);
    pPhysicsCom->Set_Speed(m_fSpeed);
    pPhysicsCom->Set_Jump(m_fJumpPower);


    __super::Enter(pOwner, pAnimator);
    pOwner->On_PlusHp(-m_fDamage);
}

STATE_TYPE CBlasted_Ground::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
    {
        if (pOwner->Get_PhysicsCom()->Get_Physics().bAir)
            return STATE_FALL_ENEMY;
        else
            return STATE_IDLE_ENEMY;

    }

    

    return __super::Tick(pOwner, pAnimator);
}

void CBlasted_Ground::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CBlasted_Ground::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{


    return STATE_END;
}
