#include "stdafx.h"
#include "CDeath_Boss.h"

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

CDeath_Boss::CDeath_Boss()
{
}

CDeath_Boss::~CDeath_Boss()
{
}

CDeath_Boss* CDeath_Boss::Create()
{
    CDeath_Boss* pInstance = new CDeath_Boss();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CDeath_Boss");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CDeath_Boss::Initialize()
{
    m_eAnimType = ANIM_BEATEN;
    m_iAnimIndex = 86;
    m_eStateType = STATE_DEATH_ENEMY;
    m_fInterPolationTime = 0.1f;

    return S_OK;
}

void CDeath_Boss::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
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

    __super::Enter(pOwner, pAnimator);
}

STATE_TYPE CDeath_Boss::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
    {
        CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Smoke", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
    }

    return __super::Tick(pOwner, pAnimator);
}

void CDeath_Boss::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */

}

STATE_TYPE CDeath_Boss::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{


    return STATE_END;
}
