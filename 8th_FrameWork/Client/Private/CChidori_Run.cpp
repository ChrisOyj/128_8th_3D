#include "stdafx.h"
#include "CChidori_Run.h"

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

#include "CEffect.h"
#include "Model.h"

#include "Functor.h"
CChidori_Run::CChidori_Run()
{
}

CChidori_Run::~CChidori_Run()
{
}

CChidori_Run* CChidori_Run::Create()
{
    CChidori_Run* pInstance = new CChidori_Run();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CChidori_Run");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

void CChidori_Run::OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos)
{
    if (eColType == COL_BODY)
    {
        if (!PLAYER->Get_TargetUnit())
            PLAYER->Set_TargetUnit((CUnit*)pOtherObj);
    }
}

HRESULT CChidori_Run::Initialize()
{
    m_eAnimType = ANIM_SKILL_SHOOT;
    m_iAnimIndex = 5;
    m_eStateType = STATE_CHIDORI_RUN;


    m_vecAdjState.push_back(STATE_LAND_PLAYER);
    m_vecAdjState.push_back(STATE_DASH_PLAYER);
    m_vecAdjState.push_back(STATE_JUMP_PLAYER);

    m_fAnimSpeed = 1.5f;
    m_fInterPolationTime = 0.1f;
    m_fStateChangableTime = 0.4f;


    return S_OK;
}

void CChidori_Run::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    CFunctor::Play_Sound(L"Eff_Dash", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

    CEffects_Factory::Get_Instance()->Create_ScreenEffects(pOwner);

    CAMERA_LERP(CAMERA_LERP_MINIZOOMRIGHT);

    pOwner->CallBack_CollisionEnter += bind(&CChidori_Run::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3);

    CUser::Get_Instance()->On_UseSkill(3);
    CPhysics* pPhysicsCom = pOwner->Get_PhysicsCom();
    pPhysicsCom->Set_MaxSpeed(pOwner->Get_Status().fDashSpeed + 20.f);
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




    CEffects_Factory::Get_Instance()->Create_LandingEffects(pOwner->Get_Transform()->Get_World(WORLD_POS), GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());

    m_EffectsList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"ChidoriRun", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));

    CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Dash_Effects_Air", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
    CEffects_Factory::Get_Instance()->Create_LandingEffects(pOwner->Get_Transform()->Get_World(WORLD_POS), GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());


    m_EffectsList.push_back(CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"ChidoBall"), pOwner));
    static_cast<CEffect*>(m_EffectsList.back())->Set_RefBone(GET_COMPONENT_FROM(pOwner, CModel)->Find_HierarchyNode("LeftHand"));
    static_cast<CEffect*>(m_EffectsList.back())->Set_OffsetPos(_float4(10.f, 10.f, 0.f, 1.f));


    LIGHTDESC			LightDesc;

    LightDesc.eType = tagLightDesc::TYPE_POINT;

    LightDesc.fRange = 5.f;

    LightDesc.vDiffuse = _float4(0.1f, 0.4f, 1.f, 1.f);
    LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
    LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
    LightDesc.pOwner = m_EffectsList.back();

    GAMEINSTANCE->Add_Light(LightDesc);
   
    

    __super::Enter(pOwner, pAnimator);

}

STATE_TYPE CChidori_Run::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    m_fChidoriLoopAcc += fDT;
    if (m_fChidoriLoopAcc > m_fChidoriLoopTime)
    {
        _float4 vChidoriPos = m_EffectsList.back()->Get_Transform()->Get_MyWorld(WORLD_POS);
        CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"ChidoriText"), vChidoriPos);

        m_fChidoriLoopAcc = 0.f;
    }


    CUnit* pTargetUnit = pOwner->Get_TargetUnit();
    CPhysics* pPhysicsCom = pOwner->Get_PhysicsCom();

    if (pTargetUnit)
    {
        _float4 vTargetPos = pTargetUnit->Get_Transform()->Get_World(WORLD_POS);
        _float4 vMyPos = pOwner->Get_Transform()->Get_World(WORLD_POS);

        _float4 vTargetDir = vTargetPos - vMyPos;

        vTargetDir.y = 0.f;
        vTargetPos -= vTargetDir * 0.5f;

        vTargetDir = vTargetPos - vMyPos;
        _float fLength = vTargetDir.Length();

        if (fLength < 1.f)
        {
            return STATE_CHIDORI_ATTACK;
        }
        pPhysicsCom->Set_MaxSpeed(pOwner->Get_Status().fDashSpeed + 25.f);
        pPhysicsCom->Set_SpeedasMax();
        pPhysicsCom->Set_Accel(150.f);
        pPhysicsCom->Set_Dir(vTargetDir);

        if (pTargetUnit->Get_PhysicsCom()->Get_Physics().bAir)
        {
            pPhysicsCom->Set_Jump(0.f);
            pPhysicsCom->Get_Physics().fGravity = 0.f;
        }

    }
    else
    {
        pPhysicsCom->Set_Dir(pOwner->Get_Transform()->Get_World(WORLD_LOOK));
        pPhysicsCom->Set_Accel(100.f);

    }


    return __super::Tick(pOwner, pAnimator);

}

STATE_TYPE CChidori_Run::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Dash으로 오는 조건
    1. 키눌렸을때
    스킬
    스킬 쿨타임도 확인하고
    스킬 챠크라 게이지도 확인하고

    */
    if (KEY(T, NONE))
    {
        return m_eStateType;
    }


    return STATE_END;
}

void CChidori_Run::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
    m_bExecuted = true;
}

void CChidori_Run::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->CallBack_CollisionEnter -= bind(&CChidori_Run::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3);

    DISABLE_GAMEOBJECT(m_EffectsList.back());

    pOwner->Get_PhysicsCom()->Set_Jump(0.f);
    pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 9.8f;


}
