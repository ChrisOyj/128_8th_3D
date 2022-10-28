#include "stdafx.h"
#include "CPose.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CScript_FollowCam.h"
CPose::CPose()
{
}

CPose::~CPose()
{
}

CPose* CPose::Create()
{
    CPose* pInstance = new CPose();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CPose");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CPose::Initialize()
{
    m_eAnimType = ANIM_WIN_TYPE_1;
    m_iAnimIndex = 0;
    m_eStateType = STATE_POSE;
    m_fInterPolationTime = 0.2f;


    return S_OK;
}

void CPose::Enter(CUnit* pOwner, CAnimator* pAnimator)
{

    __super::Enter(pOwner, pAnimator);
}

void CPose::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
}

STATE_TYPE CPose::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}
