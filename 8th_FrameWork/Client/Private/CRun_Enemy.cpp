#include "stdafx.h"
#include "CRun_Enemy.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "CCamera_Free.h"
#include "Physics.h"

#include "CUser.h"
#include "CScript_FollowCam.h"
#include "CEffects_Factory.h"
#include "CCell.h"
#include "CNavigation.h"

#include "Functor.h"

CRun_Enemy::CRun_Enemy()
{
}

CRun_Enemy::~CRun_Enemy()
{
}

CRun_Enemy* CRun_Enemy::Create()
{
    CRun_Enemy* pInstance = new CRun_Enemy();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CRun_Enemy");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CRun_Enemy::Initialize()
{
    //플레이어를 향해 달리는 상태, 
    m_iAnimIndex = 61;
    m_eStateType = STATE_RUN_ENEMY;

    //적이 나보다 높으면 점프
    m_vecAdjState.push_back(STATE_CHIDORI_ENEMY);
    m_vecAdjState.push_back(STATE_FIREBALL_ENEMY);
    m_vecAdjState.push_back(STATE_LAUGHINGMONK_ENEMY);
    m_vecAdjState.push_back(STATE_DOMEDWALL_ENEMY);
    m_vecAdjState.push_back(STATE_LEAFHURRICANE_ENEMY);

    m_vecAdjState.push_back(STATE_JUMP_ENEMY);
    m_vecAdjState.push_back(STATE_WALLJUMP_ENEMY);
    m_vecAdjState.push_back(STATE_THROW_ENEMY);

    //공격범위 안이면 공격, 대쉬 혹은 가드
    m_vecAdjState.push_back(STATE_WALK_ENEMY);
    m_vecAdjState.push_back(STATE_DASH_ENEMY);
    m_vecAdjState.push_back(STATE_GUARD_ENEMY);
    m_vecAdjState.push_back(STATE_IDLE_ENEMY);
    m_vecAdjState.push_back(STATE_HANDATTACK_1_ENEMY);


    m_fInterPolationTime = 0.2f;
    return S_OK;
}

void CRun_Enemy::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
   // CEffects_Factory::Get_Instance()->Create_LandingEffects(pOwner->Get_Transform()->Get_World(WORLD_POS), GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());

    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator);
    _float fWalkSpeed = frandom(pOwner->Get_Status().fRunSpeed, pOwner->Get_Status().fRunSpeed+2.f);

    pOwner->Get_PhysicsCom()->Set_MaxSpeed(fWalkSpeed);



}

STATE_TYPE CRun_Enemy::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    //플레이어를 추격
    m_fLoopSoundAcc += fDT;

    if (m_fLoopSoundAcc > m_fSoundLoopTime)
    {
        m_fLoopSoundAcc = 0.f;

        if (GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell()->IsGrass())
        {
            CFunctor::Play_Sound(L"Env_FootStep_Grass", CHANNEL_ENVIRONMENT, pOwner->Get_Transform()->Get_World(WORLD_POS));

        }
        else
            CFunctor::Play_Sound(L"Env_FootStep_Ground", CHANNEL_ENVIRONMENT, pOwner->Get_Transform()->Get_World(WORLD_POS));

    }

    CTransform* pTransform = pOwner->Get_Transform();
    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();
    if (pPhyscisCom->Get_Physics().bAir)
    {
        pPhyscisCom->Set_Jump();
        return STATE_FALL_ENEMY;
    }

    _float4 vCurPos = pTransform->Get_World(WORLD_POS);
    //타겟 null예외 처리는 밖에서
    _float4 vTargetPos = pOwner->Get_TargetUnit()->Get_Transform()->Get_World(WORLD_POS);

    _float4 vDir = vTargetPos - vCurPos;
    vDir.y = 0.f;
    _float4 vTargetLook = vDir.Normalize();

    _float4 vCurLook = pTransform->Get_World(WORLD_LOOK);
    _float fRatio = (vTargetLook - vCurLook).Length() * 4.f;
    vCurLook += (vTargetLook - vCurLook).Normalize() * fRatio * fDT;

    pTransform->Set_Look(vCurLook);

    pPhyscisCom->Set_Accel(30.f);
    pPhyscisCom->Set_Dir(vCurLook);

    return __super::Tick(pOwner, pAnimator);
}

void CRun_Enemy::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CRun_Enemy::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

    _float4 vCurPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
    _float4 vTargetPos = pOwner->Get_TargetUnit()->Get_Transform()->Get_World(WORLD_POS);
    vTargetPos.y = 0.f;
    vCurPos.y = 0.f;
    _float4 vDir = vTargetPos - vCurPos;
    _float fTargetLength = vDir.Length();

    //x,z로 거리가 멀면 뛰기
    if (fTargetLength > 4.f)
    {
      return m_eStateType;
    }


    return STATE_END;
}
