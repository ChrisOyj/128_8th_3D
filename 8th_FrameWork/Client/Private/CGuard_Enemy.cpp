#include "stdafx.h"
#include "CGuard_Enemy.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "Transform.h"
#include "Physics.h"
#include "CScript_FollowCam.h"
CGuard_Enemy::CGuard_Enemy()
{
}

CGuard_Enemy::~CGuard_Enemy()
{
}

CGuard_Enemy* CGuard_Enemy::Create()
{
    CGuard_Enemy* pInstance = new CGuard_Enemy();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CGuard_Enemy");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CGuard_Enemy::Initialize()
{
    m_eAnimType = ANIM_GUARD;
    m_iAnimIndex = 14;

    m_eStateType = STATE_GUARD_ENEMY;
    m_fInterPolationTime = 0.1f;
    m_fStateChangableTime = 0.5f;

    m_vecAdjState.push_back(STATE_JUMP_ENEMY);

    m_vecAdjState.push_back(STATE_RUN_ENEMY);
    m_vecAdjState.push_back(STATE_THROW_ENEMY);

    m_vecAdjState.push_back(STATE_WALK_ENEMY);
    m_vecAdjState.push_back(STATE_DASH_ENEMY);
    m_vecAdjState.push_back(STATE_IDLE_ENEMY);
    m_vecAdjState.push_back(STATE_HANDATTACK_1_ENEMY);


    return S_OK;
}

void CGuard_Enemy::Enter(CUnit* pOwner, CAnimator* pAnimator)
{


    __super::Enter(pOwner, pAnimator);
}

STATE_TYPE CGuard_Enemy::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CGuard_Enemy::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CGuard_Enemy::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    _float4 vCurPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
    _float4 vTargetPos = pOwner->Get_TargetUnit()->Get_Transform()->Get_World(WORLD_POS);
    vTargetPos.y = 0.f;
    vCurPos.y = 0.f;
    _float4 vDir = vTargetPos - vCurPos;
    _float fTargetLength = vDir.Length();


    if (fTargetLength < 4.f)
    {
        _uint iRand = random(0, 2);

        if (iRand == 0)
            return m_eStateType;
    }


    return STATE_END;
}
