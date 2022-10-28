#include "stdafx.h"
#include "CJump_NightGuy.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "CCamera_Free.h"
#include "Physics.h"

#include "CUser.h"
#include "CNavigation.h"
#include "CEffects_Factory.h"

#include "Functor.h"
CJump_NightGuy::CJump_NightGuy()
{
}

CJump_NightGuy::~CJump_NightGuy()
{
}

CJump_NightGuy* CJump_NightGuy::Create()
{
    CJump_NightGuy* pInstance = new CJump_NightGuy();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_NightGuy");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CJump_NightGuy::Initialize()
{
    m_iAnimIndex = 42;
    m_eStateType = STATE_JUMP_NIGHTGUY;

    m_vecAdjState.push_back(STATE_LAND_NIGHTGUY);
    m_vecAdjState.push_back(STATE_FALL_NIGHTGUY);
    m_vecAdjState.push_back(STATE_DASH_NIGHTGUY);
    m_vecAdjState.push_back(STATE_WALLJUMP_NIGHTGUY);
    m_vecAdjState.push_back(STATE_DOUBLEJUMP_NIGHTGUY);
    m_vecAdjState.push_back(STATE_ATTACK_1_NIGHTGUY);

    m_fInterPolationTime = 0.f;
    return S_OK;
}

void CJump_NightGuy::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    CFunctor::Play_Sound(L"Eff_Jump", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
    CFunctor::Play_Sound(L"Voice_Man_Jump", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));

    _float fJumpPower = 0.f;
    CEffects_Factory::Get_Instance()->Create_LandingEffects(pOwner->Get_Transform()->Get_World(WORLD_POS), GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());


    if (pOwner->Get_PhysicsCom()->Get_Physics().fSpeed <= 1.f)
    {
        m_iAnimIndex = 43;
        fJumpPower = pOwner->Get_Status().fJumpPower + 6.f;
    }
    else
    {
        m_iAnimIndex = 42;
        fJumpPower = pOwner->Get_Status().fJumpPower + 5.f;
    }


    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator);
    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();
    CTransform* pTransform = pOwner->Get_Transform();

   
    pTransform->Set_Y(pTransform->Get_World(WORLD_POS).y + 0.2f);
    pTransform->Make_WorldMatrix();
  


    pPhyscisCom->Set_Jump(fJumpPower);

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

    //pTransform->Set_Look(vCurLook);
    //pPhyscisCom->Set_Dir(vCurLook);
    

    pPhyscisCom->Set_MaxSpeed(pOwner->Get_Status().fRunSpeed);

    _float fMinSpeed = pOwner->Get_Status().fRunSpeed * 0.8f;

    if (pPhyscisCom->Get_Physics().fSpeed < fMinSpeed)
        pPhyscisCom->Set_Speed(fMinSpeed);

    


}

STATE_TYPE CJump_NightGuy::Tick(CUnit* pOwner, CAnimator* pAnimator)
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

    _float4 vCurLook = pTransform->Get_World(WORLD_LOOK);
    _float fRatio = (vTargetLook - vCurLook).Length() * 4.f;
    vCurLook += (vTargetLook - vCurLook).Normalize() * fRatio * fDT;

   /* _float4 vNewDir = vCurLook + (vTargetLook - vCurLook).Normalize() * fRatio * 0.01f * fDT;
    _float fCurSpeed = pPhyscisCom->Get_Physics().fSpeed;
    _float fSpeedRatio = (vTargetLook - pPhyscisCom->Get_Physics().vDir).Length();
    fCurSpeed -= fSpeedRatio * 5.f * fDT;
    pPhyscisCom->Set_Speed(fCurSpeed);
    pPhyscisCom->Set_Dir(vNewDir);*/
    pPhyscisCom->Set_Dir(vCurLook);
    pTransform->Set_Look(vCurLook);

    

    return __super::Tick(pOwner, pAnimator);
}

void CJump_NightGuy::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CJump_NightGuy::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Run으로 오는 조건
    1. 키눌렸을때
    */
    if (CUser::Get_Instance()->Get_LastKey() == KEY::SPACE)
    {
       return m_eStateType;
    }


    return STATE_END;
}
