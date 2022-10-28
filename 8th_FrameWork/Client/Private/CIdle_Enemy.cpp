#include "stdafx.h"
#include "CIdle_Enemy.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "Transform.h"
#include "CBossAura.h"
#include "CEffects_Factory.h"
#include "CScript_FollowCam.h"
CIdle_Enemy::CIdle_Enemy()
{
}

CIdle_Enemy::~CIdle_Enemy()
{
}

CIdle_Enemy* CIdle_Enemy::Create()
{
    CIdle_Enemy* pInstance = new CIdle_Enemy();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CIdle_Enemy");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CIdle_Enemy::Initialize()
{
    m_iAnimIndex = 36;
    m_eStateType = STATE_IDLE_ENEMY;
    m_fInterPolationTime = 0.1f;
    m_fStateChangableTime = 1.f;



    //적이 나보다 높으면 점프
    //m_vecAdjState.push_back(STATE_KOTOAMATSUKAMI);
    m_vecAdjState.push_back(STATE_CHIDORI_ENEMY);
    m_vecAdjState.push_back(STATE_FIREBALL_ENEMY);

    m_vecAdjState.push_back(STATE_LAUGHINGMONK_ENEMY);
    m_vecAdjState.push_back(STATE_DOMEDWALL_ENEMY);
    m_vecAdjState.push_back(STATE_LEAFHURRICANE_ENEMY);

    m_vecAdjState.push_back(STATE_JUMP_ENEMY);
    m_vecAdjState.push_back(STATE_RUN_ENEMY);
    m_vecAdjState.push_back(STATE_THROW_ENEMY);

    //공격범위 안이면 공격, 대쉬 혹은 가드
    m_vecAdjState.push_back(STATE_WALK_ENEMY);
    m_vecAdjState.push_back(STATE_DASH_ENEMY);
    m_vecAdjState.push_back(STATE_GUARD_ENEMY);
    m_vecAdjState.push_back(STATE_HANDATTACK_1_ENEMY);


    return S_OK;
}

void CIdle_Enemy::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    m_fStateChangableTime = frandom(1.f, 1.5f);
    //CGameObject* pAura = CEffects_Factory::Get_Instance()->Create_Effects(HASHCODE(CBossAura), pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
    //CUser::Get_Instance()->Set_BossAura(static_cast<CBossAura*>(pAura));

    if (pOwner->Get_Status().fMaxHP > 200.f)
    {
        //Boss

        m_fStateChangableTime = 0.1f;
    }

    __super::Enter(pOwner, pAnimator);
}

void CIdle_Enemy::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
    int i = 0;
}

STATE_TYPE CIdle_Enemy::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Idle로 오는 조건
    1. 현재 진행중인 애니메이션이 끝났을 때
    */
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
