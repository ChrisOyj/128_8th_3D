#include "stdafx.h"
#include "CNightGuy_Run.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "CCamera_Free.h"
#include "Physics.h"

#include "CUser.h"

#include "CScript_FollowCam.h"

#include "CUnit_Player.h"
#include "CState_Manager.h"
#include "CHit_Ground.h"
#include "CBlasted_Ground.h"
#include "CEffects_Factory.h"
#include "CNavigation.h"
#include "CCamera_Cinema.h"
#include "CEightGatesEffects.h"
#include "CCollider_Sphere.h"
#include "CEffect.h"
#include "Model.h"
#include "Functor.h"
CNightGuy_Run::CNightGuy_Run()
{
}

CNightGuy_Run::~CNightGuy_Run()
{
}

CNightGuy_Run* CNightGuy_Run::Create()
{
    CNightGuy_Run* pInstance = new CNightGuy_Run();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CNightGuy_Run");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

void CNightGuy_Run::OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos)
{
    CEffects_Factory::Get_Instance()->Create_ScreenEffects(pOtherObj);
    CUser::Get_Instance()->Get_FollowCam()->Start_ShakingCamera(0.1f, 0.3f);

}

HRESULT CNightGuy_Run::Initialize()
{
    m_eAnimType = ANIM_EIGHTGATES;
    m_iAnimIndex = 5;
    m_eStateType = STATE_NIGHTGUYATTACK_PLAYER;

    m_fAnimSpeed = 1.f;
    m_fInterPolationTime = 0.f;
    m_fStateChangableTime = 1.6f;

    m_eHitStateType = STATE_HIT_GROUND_ENEMY;

    return S_OK;
}

void CNightGuy_Run::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    CFunctor::Play_Sound(L"Voice_Man_NightGuyRun", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));
    CFunctor::Play_Sound(L"Eff_FireBomb", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

    pOwner->CallBack_CollisionEnter += bind(&CNightGuy_Run::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3);


    CEffects_Factory::Get_Instance()->Create_ScreenEffects(pOwner);

    CUser::Get_Instance()->Get_FollowCam()->Start_ShakingCamera(0.1f, 0.3f);

    pOwner->Get_PhysicsCom()->Set_Jump(0.f);
    pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 3.f;


    CUser::Get_Instance()->On_UseSkill(3);
    CPhysics* pPhysicsCom = pOwner->Get_PhysicsCom();
    pPhysicsCom->Set_MaxSpeed(pOwner->Get_Status().fDashSpeed + 15.f);
    pPhysicsCom->Set_SpeedasMax();
    CUnit* pTargetUnit = pOwner->Get_TargetUnit();

    if (pTargetUnit)
    {
        _float4 vTargetPos = pTargetUnit->Get_Transform()->Get_World(WORLD_POS);
        _float4 vMyPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
        _float4 vTargetDir = vTargetPos - vMyPos;
        pOwner->Get_Transform()->Set_LerpLook(vTargetDir.Normalize(), 0.3f);
        pOwner->Get_Transform()->Set_Look(vTargetDir);
        pOwner->Get_Transform()->Make_WorldMatrix();
    }
    else
    {
        _float4 vLook = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
        vLook.y = 0.f;
        vLook.Normalize();
        pOwner->Get_Transform()->Set_LerpLook(vLook, 0.3f);
        pOwner->Get_Transform()->Set_Look(vLook);
        pOwner->Get_Transform()->Make_WorldMatrix();
    }


    
    CEffects_Factory::Get_Instance()->Create_MultiEffects(L"EightGatesDash", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
    m_EffectsList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"NightGuyLoop", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));

    for (auto& elem : m_EffectsList)
    {
        static_cast<CEffect*>(elem)->Set_EffectFlag(EFFECT_FOLLOWTARGET);
    }


   

    pOwner->Enable_Collider(CUnit::RIGHTFOOT, true);
    pOwner->Get_Collider(CUnit::RIGHTFOOT)->Get_ColInfo().fRadius *= 3.f;



    CState::Enter(pOwner, pAnimator);

}

STATE_TYPE CNightGuy_Run::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    m_fChidoriLoopAcc += fDT;
    if (m_fChidoriLoopAcc > m_fChidoriLoopTime)
    {
        _float4 vChidoriPos = m_EffectsList.back()->Get_Transform()->Get_MyWorld(WORLD_POS);

        m_fChidoriLoopAcc = 0.f;
    }

    _float4 vLook = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
    CTransform* pTransform = pOwner->Get_Transform();
    CPhysics* pPhysicsCom = pOwner->Get_PhysicsCom();
    vLook.y = 0.f;
    pTransform->Set_Look(vLook);
    pPhysicsCom->Set_Dir(vLook);

    static_cast<CHit_Ground*>(CState_Manager::Get_Instance()->Get_State(STATE_HIT_GROUND_ENEMY))->On_HitGround(
        CHit_Ground::SPINBLOW, vLook * -1.f, vLook, 10.f, 12.f, 100.f);

    pPhysicsCom->Set_Accel(100.f);

    if (GET_COMPONENT_FROM(pOwner, CNavigation)->Is_Blocked()
        || GET_COMPONENT_FROM(pOwner, CNavigation)->Is_OnWall()
        || m_fTimeAcc >= m_fStateChangableTime)
    {
        DISABLE_GAMEOBJECT(PLAYER->Get_EightGatesEffect());
    }


    return CState::Tick(pOwner, pAnimator);

}

STATE_TYPE CNightGuy_Run::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Dash으로 오는 조건
    1. 키눌렸을때
    스킬
    스킬 쿨타임도 확인하고
    스킬 챠크라 게이지도 확인하고

    */
    if (pAnimator->Is_CurAnimFinished())
    {
        return m_eStateType;
    }


    return STATE_END;
}

void CNightGuy_Run::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
    m_bExecuted = true;
}

void CNightGuy_Run::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->CallBack_CollisionEnter -= bind(&CNightGuy_Run::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3);

    pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 9.8f;

    pOwner->Enable_Collider(CUnit::RIGHTFOOT, false);
    pOwner->Get_Collider(CUnit::RIGHTFOOT)->Get_ColInfo().fRadius /= 3.f;

    _float4 vLook = pOwner->Get_Transform()->Get_World(WORLD_LOOK);
    vLook.y = 0.f;
    pOwner->Get_Transform()->Set_Look(vLook);


    pOwner->Enable_BodyColliders();



    //DISABLE_GAMEOBJECT(m_EffectsList.back());

    for (auto& elem : m_EffectsList)
    {
        if (!elem->Is_Disable())
            DISABLE_GAMEOBJECT(elem);
    }


}
