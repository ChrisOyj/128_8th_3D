#include "stdafx.h"
#include "CDoubleJump_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "CCamera_Free.h"
#include "Physics.h"

#include "CUser.h"
#include "CState_Manager.h"
#include "CNavigation.h"
#include "Functor.h"
#include "CEffects_Factory.h"

CDoubleJump_Player::CDoubleJump_Player()
{
}

CDoubleJump_Player::~CDoubleJump_Player()
{
}

CDoubleJump_Player* CDoubleJump_Player::Create()
{
    CDoubleJump_Player* pInstance = new CDoubleJump_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CDoubleJump_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CDoubleJump_Player::Initialize()
{
    m_iAnimIndex = 29;
    m_eStateType = STATE_DOUBLEJUMP_PLAYER;

    m_vecAdjState.push_back(STATE_FALL_PLAYER);
    m_vecAdjState.push_back(STATE_LAND_PLAYER);
    m_vecAdjState.push_back(STATE_AIRDASHSTART_PLAYER);
    m_vecAdjState.push_back(STATE_NSAERIALATTACK_3_PLAYER);
    m_vecAdjState.push_back(STATE_HANDAERIALATTACK_4_PLAYER);
    m_vecAdjState.push_back(STATE_THROW_PLAYER);
    m_vecAdjState.push_back(STATE_RASENSURIKEN_PLAYER);
    m_vecAdjState.push_back(STATE_CHIDORI);
    m_vecAdjState.push_back(STATE_WATERPRISON_PLAYER);

    m_vecAdjState.push_back(STATE_FIREBALL_PLAYER);
    m_fInterPolationTime = 0.1f;
    return S_OK;
}

void CDoubleJump_Player::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"DoubleJump_0"), pOwner->Get_Transform()->Get_World(WORLD_POS));
    CFunctor::Play_Sound(L"Eff_Jump", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
    CFunctor::Play_Sound(L"Voice_Man_Jump", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));

    CUser::Get_Instance()->Disable_Wire();

    GET_STATE(m_eStateType, CDoubleJump_Player)->m_bCoolTime = false;

    _float fJumpPower = 7.f;
    fJumpPower = pOwner->Get_Status().fJumpPower;

    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator);
    pOwner->Get_PhysicsCom()->Set_Jump(fJumpPower);


    CTransform* pTransform = pOwner->Get_Transform();
    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();

    if (GET_COMPONENT_FROM(pOwner, CNavigation)->Is_OnWall())
    {
        pPhyscisCom->Set_Dir(pTransform->Get_World(WORLD_LOOK) * -1.f);
        pPhyscisCom->Set_MaxSpeed(pOwner->Get_Status().fRunSpeed);

        _float fMinSpeed = pOwner->Get_Status().fRunSpeed * 0.8f;

        if (pPhyscisCom->Get_Physics().fSpeed < fMinSpeed)
            pPhyscisCom->Set_Speed(fMinSpeed);
        return;
    }


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
        return;
    }
    _float4 vTargetLook = vDir.Normalize();

    _float4 vCurLook = pTransform->Get_World(WORLD_LOOK);

    vCurLook = vTargetLook;

    pTransform->Set_Look(vCurLook);
    pPhyscisCom->Set_Dir(vCurLook);


    pPhyscisCom->Set_MaxSpeed(pOwner->Get_Status().fRunSpeed);

    _float fMinSpeed = pOwner->Get_Status().fRunSpeed * 0.8f;
    pPhyscisCom->Set_Speed(fMinSpeed);
}

STATE_TYPE CDoubleJump_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
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
    pPhyscisCom->Set_Dir(vCurLook);



    return __super::Tick(pOwner, pAnimator);
}

void CDoubleJump_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CDoubleJump_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 DoubleJump로 오는 조건
    1. 키눌렸을때
    */
    if (CUser::Get_Instance()->Get_LastKey() == KEY::SPACE)
    {
        return m_eStateType;
    }


    return STATE_END;
}
