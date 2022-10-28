#include "stdafx.h"
#include "CGoTarget.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "CCamera_Free.h"
#include "Physics.h"

#include "CUser.h"
#include "CScript_FollowCam.h"

#include "CState_Manager.h"
#include "CPose.h"

CGoTarget::CGoTarget()
{
}

CGoTarget::~CGoTarget()
{
}

CGoTarget* CGoTarget::Create()
{
    CGoTarget* pInstance = new CGoTarget();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CGoTarget");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CGoTarget::Initialize()
{
    //플레이어를 향해 달리는 상태, 
    m_iAnimIndex = 61;
    m_eStateType = STATE_GOTARGET;

    m_vecAdjState.push_back(STATE_WALLJUMP_GO);

    m_fInterPolationTime = 0.2f;
    return S_OK;
}

void CGoTarget::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator);
    pOwner->Get_PhysicsCom()->Set_MaxSpeed(pOwner->Get_Status().fRunSpeed);
    m_vTargetPos.y = 0.f;

}

STATE_TYPE CGoTarget::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    //타겟포스로 달려

    CTransform* pTransform = pOwner->Get_Transform();
    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();
   
    _float4 vCurPos = pTransform->Get_World(WORLD_POS);
    _float4 vCurLook = pTransform->Get_World(WORLD_LOOK);
    vCurPos.y = 0.f;
    _float4 vDir = m_vTargetPos - vCurPos;
    _float fDistance = vDir.Length();
    vDir.Normalize();

    _float fRatio = (vDir - vCurLook).Length() * 4.f;
    vCurLook += (vDir - vCurLook).Normalize() * fRatio * fDT;


    

    pTransform->Set_Look(vCurLook);
    pPhyscisCom->Set_Dir(vCurLook);
    pPhyscisCom->Set_Accel(30.f);

    
    if (fDistance < 0.5f)
    {
        vCurLook = pOwner->Get_TargetUnit()->Get_Transform()->Get_World(WORLD_POS) - vCurPos;
        vCurLook.y = 0.f;
        pTransform->Set_LerpLook(vCurLook, 0.4f);
        CState_Manager::Get_Instance()->Get_State(STATE_POSE)->Set_AnimType(ANIM_WIN_TYPE_DANCE);

        if (CUser::Get_Instance()->Get_EventNum() > 1)
        {
            CState_Manager::Get_Instance()->Get_State(STATE_POSE)->Set_AnimType(ANIM_BOSS);
            CState_Manager::Get_Instance()->Get_State(STATE_POSE)->Set_AnimIndex(1);
        }

        return STATE_POSE;
    }


    return __super::Tick(pOwner, pAnimator);
}

void CGoTarget::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CGoTarget::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

    //x,z로 거리가 멀면 뛰기
    if (pAnimator->Is_CurAnimFinished())
    {
        return m_eStateType;
    }


    return STATE_END;
}
