#include "stdafx.h"
#include "CLand_Go.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "CCamera_Free.h"
#include "Physics.h"

#include "CState_Manager.h"
#include "CDoubleJump_Player.h"

#include "CNavigation.h"

#include "CUser.h"
#include "CScript_FollowCam.h"

#include "CEffects_Factory.h"

CLand_Go::CLand_Go()
{
}

CLand_Go::~CLand_Go()
{
}

CLand_Go* CLand_Go::Create()
{
    CLand_Go* pInstance = new CLand_Go();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CLand_Go");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CLand_Go::Initialize()
{
    m_iAnimIndex = 37;
    m_eStateType = STATE_LAND_GO;

    m_vecAdjState.push_back(STATE_GOTARGET);

    m_fInterPolationTime = 0.05f;
    m_fStateChangableTime = 0.2f;
    m_fDelayTime = 0.55f;

    return S_OK;
}

void CLand_Go::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    CEffects_Factory::Get_Instance()->Create_LandingEffects(pOwner->Get_Transform()->Get_World(WORLD_POS), GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());

    /* OwnerÀÇ Animator Set Idle·Î */
    __super::Enter(pOwner, pAnimator);

}

STATE_TYPE CLand_Go::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);
}

void CLand_Go::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CLand_Go::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    if (!pOwner->Get_PhysicsCom()->Get_Physics().bAir)
        return m_eStateType;


    return STATE_END;
}
