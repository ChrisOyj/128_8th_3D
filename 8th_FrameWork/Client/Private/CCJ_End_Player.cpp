#include "stdafx.h"
#include "CCJ_End_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "Transform.h"
#include "Physics.h"
#include "CCamera_Follow.h"

#include "CUser.h"
#include "CScript_FollowCam.h"

#include "CEffects_Factory.h"
#include "CScreenEffect.h"
#include "Functor.h"

CCJ_End_Player::CCJ_End_Player()
{
}

CCJ_End_Player::~CCJ_End_Player()
{
}

CCJ_End_Player* CCJ_End_Player::Create()
{
    CCJ_End_Player* pInstance = new CCJ_End_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CCJ_End_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CCJ_End_Player::Initialize()
{
    m_iAnimIndex = 16;
    m_eStateType = STATE_CJ_END_PLAYER;
    m_fInterPolationTime = 0.f;


    m_vecAdjState.push_back(STATE_AIRDASHLOOP_PLAYER);
    m_vecAdjState.push_back(STATE_LAND_PLAYER);


    return S_OK;
}

void CCJ_End_Player::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    CFunctor::Play_Sound(L"Eff_ChakraJumpEnd", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
    CFunctor::Play_Sound(L"Voice_Man_CJ", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));

    CEffects_Factory::Get_Instance()->Create_ScreenEffects(pOwner);

    CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_DASH);

    /* Owner의 Animator Set Idle로 */
    //여기서
    CTransform* pTransform = pOwner->Get_Transform();
    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();

    m_fChargeTime = max(m_fChargeTime * 2.f, 1.5f);

    _float fDashSpeed = pOwner->Get_Status().fRunSpeed * m_fChargeTime;
    _float fJumpPower = pOwner->Get_Status().fJumpPower * m_fChargeTime * 1.1f;
    _float fFricRatio = 3.f * m_fChargeTime;

    _float4 vUserLook = CGameInstance::Get_Instance()->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
    vUserLook.y = 0.f;
    vUserLook.Normalize();

    _float4 _vLook = vUserLook;
    _float4 vUp = pTransform->Get_World(WORLD_UP);
    _float4 vRight = vUp.Cross(_vLook);

    _float4 vDir = { 0.f, 0.f, 0.f, 0.f };
    if (KEY(W, HOLD))
    {
        vDir += _vLook;
    }
    if (KEY(A, HOLD))
    {
        vDir -= vRight;

    }
    if (KEY(D, HOLD))
    {
        vDir += vRight;
    }

    if (KEY(S, HOLD))
    {
        vDir -= _vLook;

    }

    pPhyscisCom->Set_Jump(fJumpPower);
    pPhyscisCom->Get_PhysicsDetail().fFrictionRatio = fFricRatio;


    if (vDir.Is_Zero())
    {
        __super::Enter(pOwner, pAnimator);
        CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Dash_Effects_Air", pOwner, pTransform->Get_World(WORLD_POS));
        return;
    }

    pPhyscisCom->Set_MaxSpeed(fDashSpeed);
    pPhyscisCom->Set_Speed(fDashSpeed);
    _float4 vTargetLook = vDir.Normalize();
    pTransform->Set_Look(vTargetLook);
    pPhyscisCom->Set_Dir(vTargetLook);
    CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Dash_Effects_Air", pOwner, pTransform->Get_World(WORLD_POS));




    __super::Enter(pOwner, pAnimator);
}

void CCJ_End_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */

}

STATE_TYPE CCJ_End_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Idle로 오는 조건
    1. 현재 진행중인 애니메이션이 끝났을 때
    */
    if (KEY(C, NONE))
        return m_eStateType;


    return STATE_END;
}
