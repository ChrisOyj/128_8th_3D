#include "stdafx.h"
#include "CWalk_NightGuy.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "Physics.h"
#include "CCamera_Free.h"


CWalk_NightGuy::CWalk_NightGuy()
{
}

CWalk_NightGuy::~CWalk_NightGuy()
{
}

CWalk_NightGuy* CWalk_NightGuy::Create()
{
    CWalk_NightGuy* pInstance = new CWalk_NightGuy();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWalk_NightGuy");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CWalk_NightGuy::Initialize()
{
    m_iAnimIndex = 71;
    m_eStateType = STATE_WALK_NIGHTGUY;

    m_vecAdjState.push_back(STATE_WALK_END_NIGHTGUY);
    m_vecAdjState.push_back(STATE_DASH_NIGHTGUY);
    m_vecAdjState.push_back(STATE_RUN_NIGHTGUY);
    m_vecAdjState.push_back(STATE_JUMP_NIGHTGUY);
    m_vecAdjState.push_back(STATE_ATTACK_1_NIGHTGUY);
    m_vecAdjState.push_back(STATE_NIGHTGUY_PLAYER);

    m_fInterPolationTime = 0.2f;
    return S_OK;
}

void CWalk_NightGuy::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator);

    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();
    pPhyscisCom->Set_MaxSpeed(pOwner->Get_Status().fWalkSpeed);
}

STATE_TYPE CWalk_NightGuy::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    CTransform* pTransform = pOwner->Get_Transform();
    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();

    if (pPhyscisCom->Get_Physics().bAir)
    {
        pPhyscisCom->Set_Jump();
        return STATE_FALL_NIGHTGUY;
    }

    _float4 vUserLook = CGameInstance::Get_Instance()->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
    vUserLook.Normalize();

    _float4 _vLook = vUserLook;
    _float4 vUp = pTransform->Get_World(WORLD_UP);
    _float4 vRight = vUp.Cross(_vLook);
    _vLook = vRight.Cross(vUp.Normalize());
    _vLook.Normalize();

    _float4 vDir = { 0.f, 0.f, 0.f, 0.f };
    if (KEY(A, HOLD))
    {
        vDir -= vRight;
    }
    if (KEY(D, HOLD))
    {
        vDir += vRight;
    }
    if (KEY(W, HOLD))
    {
        vDir += _vLook;
    }
    if (KEY(S, HOLD))
    {
        vDir -= _vLook;
    }

    if (vDir.Is_Zero())
    {
        return __super::Tick(pOwner, pAnimator);
    }

    _float4 vTargetLook = vDir.Normalize();

    _float4 vCurPos = pTransform->Get_World(WORLD_POS);
    _float4 vCurLook = pTransform->Get_World(WORLD_LOOK);

    _float fRatio = (vTargetLook - vCurLook).Length() * 4.f;

    vCurLook += (vTargetLook - vCurLook).Normalize() * fRatio * fDT;


    vRight = vUp.Cross(vCurLook);

    pTransform->Set_World(WORLD_LOOK, vCurLook);
    pTransform->Set_World(WORLD_UP, vUp);
    pTransform->Set_World(WORLD_RIGHT, vRight.Normalize());

    pPhyscisCom->Set_Accel(30.f);
    pPhyscisCom->Set_Dir(vCurLook);

    return __super::Tick(pOwner, pAnimator);
}

void CWalk_NightGuy::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CWalk_NightGuy::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Walk로 오는 조건 
    1. 키눌렸을때
    */
    if (!(KEY(LSHIFT, HOLD)))
    {
        if (KEY(W, HOLD) ||
            KEY(A, HOLD) ||
            KEY(S, HOLD) ||
            KEY(D, HOLD))
        {
            return m_eStateType;
        }
    }


    return STATE_END;
}
