#include "stdafx.h"
#include "CJump_Enemy.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "CCamera_Free.h"
#include "Physics.h"

#include "CUser.h"
#include "CNavigation.h"
#include "CEffects_Factory.h"
#include "CCell.h"

#include "Functor.h"
CJump_Enemy::CJump_Enemy()
{
}

CJump_Enemy::~CJump_Enemy()
{
}

CJump_Enemy* CJump_Enemy::Create()
{
    CJump_Enemy* pInstance = new CJump_Enemy();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_Enemy");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CJump_Enemy::Initialize()
{
    m_iAnimIndex = 42;
    m_eStateType = STATE_JUMP_ENEMY;

    m_vecAdjState.push_back(STATE_WALLJUMP_ENEMY);
    m_vecAdjState.push_back(STATE_FALL_ENEMY);
    m_vecAdjState.push_back(STATE_LAND_ENEMY);
    m_vecAdjState.push_back(STATE_THROW_ENEMY);

    //가까우면 공격
    m_vecAdjState.push_back(STATE_HANDAERIALATTACK_4_ENEMY);

    m_fInterPolationTime = 0.f;
    m_fStateChangableTime = 0.2f;
    return S_OK;
}

void CJump_Enemy::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    CFunctor::Play_Sound(L"Eff_Jump", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

    CEffects_Factory::Get_Instance()->Create_LandingEffects(pOwner->Get_Transform()->Get_World(WORLD_POS), GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());

    _float fJumpPower = 0.f;

    if (pOwner->Get_PhysicsCom()->Get_Physics().fSpeed <= 1.f)
    {
        m_iAnimIndex = 43;
        fJumpPower = pOwner->Get_Status().fJumpPower + 1.f;
    }
    else
    {
        m_iAnimIndex = 42;
        fJumpPower = pOwner->Get_Status().fJumpPower;
    }


    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator);
    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();
    CTransform* pTransform = pOwner->Get_Transform();


    pTransform->Set_Y(pTransform->Get_World(WORLD_POS).y + 0.2f);
    pTransform->Make_WorldMatrix();



    pPhyscisCom->Set_Jump(fJumpPower);

    pPhyscisCom->Set_MaxSpeed(pOwner->Get_Status().fRunSpeed);

    _float fMinSpeed = pOwner->Get_Status().fRunSpeed * 0.8f;
    pPhyscisCom->Set_Speed(fMinSpeed);




}

STATE_TYPE CJump_Enemy::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    CTransform* pTransform = pOwner->Get_Transform();
    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();

    _float4 vCurPos = pTransform->Get_World(WORLD_POS);
    //타겟 null예외 처리는 밖에서
    _float4 vTargetPos = pOwner->Get_TargetUnit()->Get_Transform()->Get_World(WORLD_POS);

    _float4 vDir = vTargetPos - vCurPos;
    _float fTargetLength = vDir.Length();
    _float4 vTargetLook = vDir.Normalize();

    _float4 vCurLook = pTransform->Get_World(WORLD_LOOK);
    _float fRatio = (vTargetLook - vCurLook).Length() * 4.f;
    vCurLook += (vTargetLook - vCurLook).Normalize() * fRatio * fDT;
    pPhyscisCom->Set_Dir(vCurLook);
    pTransform->Set_Look(vCurLook);



    return __super::Tick(pOwner, pAnimator);
}

void CJump_Enemy::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CJump_Enemy::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 플레이어가 나보다 높이 있으면 점프
    */

    _float4 vCurPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
    _float4 vTargetPos = pOwner->Get_TargetUnit()->Get_Transform()->Get_World(WORLD_POS);

    _float fPlayerY = vTargetPos.y;
    _float fMyY = vCurPos.y;

    vTargetPos.y = 0.f;
    vCurPos.y = 0.f;
    _float4 vDir = vTargetPos - vCurPos;
    _float fTargetLength = vDir.Length();

    //x,z로 거리가 가까운데 높으면 뛰기
    if (fTargetLength < 7.f && (fPlayerY - fMyY) > 3.f)
    {
        return m_eStateType;
    }


    return STATE_END;
}
