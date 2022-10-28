#include "stdafx.h"
#include "CThrow_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "CCamera_Free.h"
#include "Physics.h"

#include "CUser.h"

#include "CScript_FollowCam.h"
#include "CEffects_Factory.h"
#include "CKunai.h"
#include "CCollider_Sphere.h"
#include "Model.h"
#include "CWindmill.h"

#include "CNavigation.h"

#include "CCell.h"

#include "CBossKunai.h"
#include "Functor.h"

CThrow_Player::CThrow_Player()
{
}

CThrow_Player::~CThrow_Player()
{
}

CThrow_Player* CThrow_Player::Create()
{
    CThrow_Player* pInstance = new CThrow_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CThrow_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CThrow_Player::Initialize()
{
    //left 1, right 2, air 3, air windmill 17, default 19, moving windmill 34

    m_eAnimType = ANIM_THROW;
    m_iAnimIndex = 19;
    m_eStateType = STATE_THROW_PLAYER;


    m_vecAdjState.push_back(STATE_IDLE_PLAYER);
    m_vecAdjState.push_back(STATE_DASH_PLAYER);
    m_vecAdjState.push_back(STATE_RUN_PLAYER);
    m_vecAdjState.push_back(STATE_JUMP_PLAYER);
    m_vecAdjState.push_back(STATE_CJ_START_PLAYER);
    m_vecAdjState.push_back(STATE_NSATTACK_1_PLAYER);
    m_vecAdjState.push_back(STATE_HANDATTACK_DASH_PLAYER);
    m_vecAdjState.push_back(STATE_HANDATTACK_POWER_PLAYER);
    m_vecAdjState.push_back(STATE_GUARD_PLAYER);
    m_vecAdjState.push_back(STATE_THROW_PLAYER);
    m_vecAdjState.push_back(STATE_RASENSURIKEN_PLAYER);
    m_vecAdjState.push_back(STATE_CHIDORI);
    m_vecAdjState.push_back(STATE_FIREBALL_PLAYER);


    m_fInterPolationTime = 0.05f;
    m_fStateChangableTime = 0.7f;
    m_fAnimSpeed = 1.1f;

    return S_OK;
}

void CThrow_Player::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    //left 1, right 2, air 3, air windmill 17, default 19, moving windmill 34

    CFunctor::Play_Sound(L"Voice_Man_StrongAttack", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));

    CUser::Get_Instance()->On_UseSkill(1);

    m_iAnimIndex = 19;
    m_fDelayTime = 0.4f;

    _float fDashSpeed = pOwner->Get_Status().fShortDashSpeed;

    CTransform* pTransform = pOwner->Get_Transform();
    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();

    _float4 vUserLook = CGameInstance::Get_Instance()->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
    vUserLook.y = 0.f;
    vUserLook.Normalize();
    _float4 _vLook = vUserLook;
    _float4 vUp = _float4(0.f, 1.f, 0.f, 0.f);
    _float4 vRight = vUp.Cross(_vLook);
    _vLook = vRight.Cross(vUp.Normalize());
    _vLook.Normalize();

    _float4 vDir = { 0.f, 0.f, 0.f, 0.f };

    if (pOwner->Is_Hand())
    {
        m_pWindmill = CEffects_Factory::Get_Instance()->Create_Windmill(
            pOwner,
            GET_COMPONENT_FROM(pOwner, CModel)->Find_HierarchyNode("RightHand"));
        GET_COMPONENT_FROM(m_pWindmill, CNavigation)->Set_StartCell(GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());


        m_fDelayTime = 0.52f;
        m_fStateChangableTime = 0.65f;
        m_iAnimIndex = 34;


        //풍마수리검
        if (pPhyscisCom->Get_Physics().bAir)
        {
            m_iAnimIndex = 17;
            pPhyscisCom->Set_Jump(0.f);
            pPhyscisCom->Get_Physics().fGravity = 0.f;
        }
        else if (KEY(A, HOLD))
        {
            vDir -= vRight;
            m_iAnimIndex = 34;
            CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_RIGHT);
        }
        else if (KEY(D, HOLD))
        {
            vDir += vRight;
            m_iAnimIndex = 34;
            CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_LEFT);
        }
        else if (KEY(S, HOLD))
        {
            vDir -= _vLook;
            m_iAnimIndex = 34;
            CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_BACKDASH);
        }
    }
    else
    {
        if (pPhyscisCom->Get_Physics().bAir)
        {
            m_fDelayTime = 0.25f;
            m_fStateChangableTime = 0.5f;
            m_iAnimIndex = 3;
            pPhyscisCom->Set_Jump(5.f);
        }
        else if (KEY(A, HOLD))
        {
            m_fDelayTime = 0.25f;
            m_fStateChangableTime = 0.6f;
            vDir -= vRight;
            m_iAnimIndex = 1;
            CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_RIGHT);
        }
        else if (KEY(D, HOLD))
        {
            m_fDelayTime = 0.25f;
            m_fStateChangableTime = 0.6f;
            vDir += vRight;
            m_iAnimIndex = 2;
            CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_LEFT);
        }

        
    }


    //카메라쪽 바라봐
    pTransform->Set_LerpLook(_vLook, 0.4f);

    //안눌렸으면 이동 x
    if (vDir.Is_Zero())
    {
        __super::Enter(pOwner, pAnimator);
        return;
    }

    if (m_iAnimIndex <= 2 || m_iAnimIndex == 34)
    {
        CFunctor::Play_Sound(L"Eff_GroundDash", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));

    }

    pPhyscisCom->Set_MaxSpeed(fDashSpeed);
    pPhyscisCom->Set_Speed(fDashSpeed);
    pPhyscisCom->Set_Jump(3.f);
    pPhyscisCom->Set_Dir(vDir.Normalize());
    pPhyscisCom->Get_PhysicsDetail().fFrictionRatio = 0.5f;

    __super::Enter(pOwner, pAnimator);

}

STATE_TYPE CThrow_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_fStateChangableTime <= m_fTimeAcc + fDT &&
        pOwner->Get_PhysicsCom()->Get_Physics().bAir)
    {
        return STATE_FALL_PLAYER;
    }

    return __super::Tick(pOwner, pAnimator);
}

void CThrow_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
    CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_DEFAULT);

    if (pOwner->Get_PhysicsCom()->Get_Physics().bAir)
        pOwner->Get_PhysicsCom()->Set_Speed(pOwner->Get_PhysicsCom()->Get_Physics().fSpeed * 0.7f);

    if (m_pWindmill)
    {
        DISABLE_GAMEOBJECT(m_pWindmill);
    }

    pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 9.8f;


}

STATE_TYPE CThrow_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Dash으로 오는 조건
    1. 키눌렸을때
    스킬
    스킬 쿨타임도 확인하고
    스킬 챠크라 게이지도 확인하고

    */
    if (CUser::Get_Instance()->Get_LastKey() == KEY::F &&
        CUser::Get_Instance()->Can_UseSkill(1))
    {
        return m_eStateType;
    }


    return STATE_END;
}

void CThrow_Player::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
    CFunctor::Play_Sound(L"Eff_Throw", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

    if (!pOwner->Get_PhysicsCom()->Get_Physics().bAir)
        CEffects_Factory::Get_Instance()->Create_LandingEffects(pOwner->Get_Transform()->Get_World(WORLD_POS), GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());

    CTransform* pTransform = pOwner->Get_Transform();
    _float4 vMyPos = pTransform->Get_World(WORLD_POS);
    vMyPos.y += 0.5f;
    _float4 vLook = pTransform->Get_World(WORLD_LOOK);
    _float4 vTargetPos;

    if (m_iAnimIndex == 17 || m_iAnimIndex == 34)
    {
        if (m_iAnimIndex == 17)
            pOwner->Get_PhysicsCom()->Set_Jump(5.f);

        //풍마수리검
        if (m_pWindmill)
        {
            m_pWindmill->On_Shoot(pOwner->Get_TargetUnit(), vMyPos + vLook * 15.f);
            m_pWindmill = nullptr;
        }

        pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 9.8f;

    }
    else
    {
        if (m_iAnimIndex == 3)
        {
            pOwner->Get_PhysicsCom()->Set_Jump(3.f);
            //pOwner->Get_PhysicsCom()->Set_Speed(7.f);
            vMyPos.y += 0.6f;

        }
        if (pOwner->Get_TargetUnit())
        {
            //타겟이 있으면 타겟쪽으로 수리검 생성
            vTargetPos = pOwner->Get_TargetUnit()->Get_Transform()->Get_World(WORLD_POS);
            vTargetPos.y += 0.5f;

            //아니면 바라보는 곳으로 걍 생성
        }
        else
        {
            vTargetPos = vMyPos + vLook * 30.f;
        }

        if (pOwner->Is_Water())
        {
            CGameObject* pGameObejct = CEffects_Factory::Get_Instance()->Create_Effects(HASHCODE(CBossKunai), vMyPos, vTargetPos);
            static_cast<CBossKunai*>(pGameObejct)->Set_ColType(COL_PLAYERSKILL);
            if (pOwner->Get_TargetUnit())
               static_cast<CEffect*>(pGameObejct)->Set_FollowTarget(pOwner->Get_TargetUnit());
            GET_COMPONENT_FROM(pGameObejct, CNavigation)->Set_StartCell(GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());
        }
        else
        {
            CGameObject* pKunai = CEffects_Factory::Get_Instance()->Create_Effects(HASHCODE(CKunai), vMyPos, vTargetPos);
            GET_COMPONENT_FROM(pKunai, CCollider_Sphere)->Set_ColIndex(COL_PLAYERTHROW);
            GET_COMPONENT_FROM(pKunai, CNavigation)->Set_StartCell(GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());
        }
      

    }

    

    

}
