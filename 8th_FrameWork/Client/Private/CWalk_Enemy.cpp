#include "stdafx.h"
#include "CWalk_Enemy.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "CCamera_Free.h"
#include "Physics.h"

#include "CUser.h"
#include "CScript_FollowCam.h"

CWalk_Enemy::CWalk_Enemy()
{
}

CWalk_Enemy::~CWalk_Enemy()
{
}

CWalk_Enemy* CWalk_Enemy::Create()
{
    CWalk_Enemy* pInstance = new CWalk_Enemy();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWalk_Enemy");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CWalk_Enemy::Initialize()
{
    //플레이어를 향해 달리는 상태, 
    m_iAnimIndex = 71;
    m_eStateType = STATE_WALK_ENEMY;

    //적이 나보다 높으면 점프
    m_vecAdjState.push_back(STATE_CHIDORI_ENEMY);
    m_vecAdjState.push_back(STATE_FIREBALL_ENEMY);

    m_vecAdjState.push_back(STATE_LAUGHINGMONK_ENEMY);
    m_vecAdjState.push_back(STATE_DOMEDWALL_ENEMY);
    m_vecAdjState.push_back(STATE_LEAFHURRICANE_ENEMY);
    m_vecAdjState.push_back(STATE_JUMP_ENEMY);
    m_vecAdjState.push_back(STATE_THROW_ENEMY);
    m_vecAdjState.push_back(STATE_RUN_ENEMY);

    //공격범위 안이면 공격, 대쉬 혹은 가드
    m_vecAdjState.push_back(STATE_WALK_ENEMY);
    m_vecAdjState.push_back(STATE_DASH_ENEMY);
    m_vecAdjState.push_back(STATE_GUARD_ENEMY);
    m_vecAdjState.push_back(STATE_IDLE_ENEMY);
    m_vecAdjState.push_back(STATE_HANDATTACK_1_ENEMY);


    m_fStateChangableTime = 2.f;
    m_fInterPolationTime = 0.15f;
    return S_OK;
}

void CWalk_Enemy::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Owner의 Animator Set Idle로 */
    m_fStateChangableTime = frandom(2.f, 3.f);

    if (pOwner->Get_Status().fMaxHP > 200.f)
    {
        //Boss

        m_fStateChangableTime = 1.f;
    }

    __super::Enter(pOwner, pAnimator);
    _float fWalkSpeed = frandom(1.f, 2.f);
    pOwner->Get_PhysicsCom()->Set_MaxSpeed(fWalkSpeed);

    _uint iRand = random(0, 1);
    if (iRand == 0)
        m_fSign = -1.f;


}

STATE_TYPE CWalk_Enemy::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    //플레이어를 추격

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
    
    pPhyscisCom->Set_Dir(pTransform->Get_MyWorld(WORLD_RIGHT) * m_fSign);

    return __super::Tick(pOwner, pAnimator);
}

void CWalk_Enemy::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CWalk_Enemy::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    _float4 vCurPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
    _float4 vTargetPos = pOwner->Get_TargetUnit()->Get_Transform()->Get_World(WORLD_POS);
    vTargetPos.y = 0.f;
    vCurPos.y = 0.f;
    _float4 vDir = vTargetPos - vCurPos;
    _float fTargetLength = vDir.Length();


    if (fTargetLength < 4.f)
    {
        _uint iRand = random(0, 1);

        if (iRand == 0)
            return m_eStateType;
    }


    return STATE_END;
}
