#include "stdafx.h"
#include "CFall_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "CCamera_Free.h"
#include "Physics.h"

CFall_Player::CFall_Player()
{
}

CFall_Player::~CFall_Player()
{
}

CFall_Player* CFall_Player::Create()
{
    CFall_Player* pInstance = new CFall_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CFall_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CFall_Player::Initialize()
{
    m_iAnimIndex = 30;
    m_eStateType = STATE_FALL_PLAYER;

    m_vecAdjState.push_back(STATE_LAND_PLAYER);
    m_vecAdjState.push_back(STATE_WALLJUMP_PLAYER);
    m_vecAdjState.push_back(STATE_DOUBLEJUMP_PLAYER);
    m_vecAdjState.push_back(STATE_AIRDASHSTART_PLAYER);
    m_vecAdjState.push_back(STATE_NSAERIALATTACK_1_PLAYER);
    m_vecAdjState.push_back(STATE_HANDAERIALATTACK_1_PLAYER);
    m_vecAdjState.push_back(STATE_NSAERIALATTACK_3_PLAYER);
    m_vecAdjState.push_back(STATE_HANDAERIALATTACK_4_PLAYER);
    m_vecAdjState.push_back(STATE_THROW_PLAYER);
    m_vecAdjState.push_back(STATE_RASENSURIKEN_PLAYER);
    m_vecAdjState.push_back(STATE_CHIDORI);
    m_vecAdjState.push_back(STATE_FIREBALL_PLAYER);

    m_vecAdjState.push_back(STATE_WATERPRISON_PLAYER);
    m_fInterPolationTime = 0.2f;
    return S_OK;
}

void CFall_Player::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pOwner->Get_PhysicsCom()->Get_Physics().fSpeed <= 1.f)
    {
        m_iAnimIndex = 31;
    }
    else
    {
        m_iAnimIndex = 30;
    }

    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator);

}

STATE_TYPE CFall_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    CTransform* pTransform = pOwner->Get_Transform();
    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();

    _float4 vUserLook = CGameInstance::Get_Instance()->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
    vUserLook.y = 0.f;
    vUserLook.Normalize();

    _float4 _vLook = vUserLook;
    _float4 vUp = { 0.f, 1.f, 0.f, 0.f };
    _float4 vRight = vUp.Cross(_vLook);
    vUp = _vLook.Cross(vRight.Normalize());

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

    pTransform->Set_Look(vCurLook);

    if (pPhyscisCom->Get_Physics().fSpeed < pOwner->Get_Status().fRunSpeed)
    {
        pPhyscisCom->Set_Accel(10.f);
    }
    pPhyscisCom->Set_Dir(vCurLook);


    return __super::Tick(pOwner, pAnimator);
}

void CFall_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CFall_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Run으로 오는 조건
    1. 키눌렸을때
    */
    if (pAnimator->Is_CurAnimFinished())
        return m_eStateType;


    return STATE_END;
}
