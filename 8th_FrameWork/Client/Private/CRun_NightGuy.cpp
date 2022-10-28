#include "stdafx.h"
#include "CRun_NightGuy.h"

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
CRun_NightGuy::CRun_NightGuy()
{
}

CRun_NightGuy::~CRun_NightGuy()
{
}

CRun_NightGuy* CRun_NightGuy::Create()
{
    CRun_NightGuy* pInstance = new CRun_NightGuy();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CRun_NightGuy");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CRun_NightGuy::Initialize()
{
    m_iAnimIndex = 61;
    m_eStateType = STATE_RUN_NIGHTGUY;

    m_vecAdjState.push_back(STATE_RUN_END_NIGHTGUY);
    m_vecAdjState.push_back(STATE_DASH_NIGHTGUY);
    m_vecAdjState.push_back(STATE_WALLJUMP_NIGHTGUY);
    m_vecAdjState.push_back(STATE_JUMP_NIGHTGUY);
    m_vecAdjState.push_back(STATE_ATTACK_1_NIGHTGUY);
    m_vecAdjState.push_back(STATE_NIGHTGUY_PLAYER);

    m_fInterPolationTime = 0.2f;
    m_fAnimSpeed = 1.3f;

    return S_OK;
}

void CRun_NightGuy::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Owner의 Animator Set Idle로 */
    //CEffects_Factory::Get_Instance()->Create_LandingEffects(pOwner->Get_Transform()->Get_World(WORLD_POS), GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());

    __super::Enter(pOwner, pAnimator);
    pOwner->Get_PhysicsCom()->Set_MaxSpeed(pOwner->Get_Status().fRunSpeed * 1.5f);
    CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_RUN);


}

STATE_TYPE CRun_NightGuy::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
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

    pPhyscisCom->Set_Accel(50.f);
    pPhyscisCom->Set_Dir(vCurLook);

    return __super::Tick(pOwner, pAnimator);
}

void CRun_NightGuy::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
    CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_DEFAULT);

}

STATE_TYPE CRun_NightGuy::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Run으로 오는 조건
    1. 키눌렸을때
    */
    if (KEY(LSHIFT, HOLD))
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
