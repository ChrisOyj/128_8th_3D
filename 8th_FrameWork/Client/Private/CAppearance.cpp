#include "stdafx.h"
#include "CAppearance.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CScript_FollowCam.h"
#include "CEffects_Factory.h"
#include "CNavigation.h"
#include "Transform.h"

#include "Functor.h"
#include "CCell.h"

CAppearance::CAppearance()
{
}

CAppearance::~CAppearance()
{
}

CAppearance* CAppearance::Create()
{
    CAppearance* pInstance = new CAppearance();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CAppearance");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CAppearance::Initialize()
{
    m_eAnimType = ANIM_APPEARANCE;
    m_iAnimIndex = 0;
    m_eStateType = STATE_APPEARANCE;
    m_fInterPolationTime = 0.05f;
    m_fDelayTime = 0.1f;


    return S_OK;
}

void CAppearance::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    m_fAnimSpeed = frandom(0.5f, 1.f);

    __super::Enter(pOwner, pAnimator);
}

STATE_TYPE CAppearance::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
    {
        return STATE_RUN_ENEMY;
    }

    return STATE_END;
}

void CAppearance::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
}

STATE_TYPE CAppearance::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

void CAppearance::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
    CEffects_Factory::Get_Instance()->Create_LandingEffects(pOwner->Get_Transform()->Get_World(WORLD_POS), GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());
    if (GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell()->IsGrass())
    {
        CFunctor::Play_Sound(L"Env_Landing_Grass", CHANNEL_ENVIRONMENT, pOwner->Get_Transform()->Get_World(WORLD_POS));
    }
    else
        CFunctor::Play_Sound(L"Env_Landing_Ground", CHANNEL_ENVIRONMENT, pOwner->Get_Transform()->Get_World(WORLD_POS));
}
