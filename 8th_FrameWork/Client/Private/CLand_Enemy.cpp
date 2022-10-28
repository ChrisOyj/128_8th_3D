#include "stdafx.h"
#include "CLand_Enemy.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "CCamera_Free.h"
#include "Physics.h"

#include "CState_Manager.h"
#include "CDoubleJump_Player.h"

#include "CNavigation.h"

#include "CUser.h"
#include "CScript_FollowCam.h"
#include "CEffects_Factory.h"
#include "CCell.h"
#include "Functor.h"
CLand_Enemy::CLand_Enemy()
{
}

CLand_Enemy::~CLand_Enemy()
{
}

CLand_Enemy* CLand_Enemy::Create()
{
    CLand_Enemy* pInstance = new CLand_Enemy();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CLand_Enemy");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CLand_Enemy::Initialize()
{
    m_iAnimIndex = 37;
    m_eStateType = STATE_LAND_ENEMY;

    m_vecAdjState.push_back(STATE_CHIDORI_ENEMY);
    m_vecAdjState.push_back(STATE_FIREBALL_ENEMY);
    m_vecAdjState.push_back(STATE_LAUGHINGMONK_ENEMY);
    m_vecAdjState.push_back(STATE_DOMEDWALL_ENEMY);
    m_vecAdjState.push_back(STATE_LEAFHURRICANE_ENEMY);

    m_vecAdjState.push_back(STATE_RUN_ENEMY);
    m_vecAdjState.push_back(STATE_JUMP_ENEMY);
    m_vecAdjState.push_back(STATE_THROW_ENEMY);

    m_vecAdjState.push_back(STATE_WALK_ENEMY);
    m_vecAdjState.push_back(STATE_DASH_ENEMY);
    m_vecAdjState.push_back(STATE_GUARD_ENEMY);
    m_vecAdjState.push_back(STATE_IDLE_ENEMY);
    m_vecAdjState.push_back(STATE_HANDATTACK_1_ENEMY);

    m_fInterPolationTime = 0.05f;
    m_fStateChangableTime = 0.2f;
    m_fDelayTime = 0.55f;

    return S_OK;
}

void CLand_Enemy::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    if (GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell()->IsGrass())
    {
        CFunctor::Play_Sound(L"Env_Landing_Grass", CHANNEL_ENVIRONMENT, pOwner->Get_Transform()->Get_World(WORLD_POS));
    }
    else
        CFunctor::Play_Sound(L"Env_Landing_Ground", CHANNEL_ENVIRONMENT, pOwner->Get_Transform()->Get_World(WORLD_POS));

    CEffects_Factory::Get_Instance()->Create_LandingEffects(pOwner->Get_Transform()->Get_World(WORLD_POS), GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());

    m_iAnimIndex = 37;
    m_fAnimSpeed = 1.f;
    m_fStateChangableTime = 0.2f;

    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();
    pPhyscisCom->Get_PhysicsDetail().fFrictionRatio = 1.f;

    /*if (pPhyscisCom->Get_Physics().fAcc >= 3.f || pPhyscisCom->Get_Physics().fSpeed > pOwner->Get_Status().fRunSpeed)
    {
        m_iAnimIndex = 35;
        m_fAnimSpeed = 1.3f;
        m_fStateChangableTime = 0.35f;

        pPhyscisCom->Set_MaxSpeed(pOwner->Get_Status().fShortDashSpeed);
        pPhyscisCom->Set_Speed(pOwner->Get_Status().fShortDashSpeed);
        pPhyscisCom->Get_PhysicsDetail().fFrictionRatio = 0.5f;



    }*/


    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator);

    m_fTimeAcc = 0.f;


    CTransform* pTransform = pOwner->Get_Transform();



    CNavigation* pNavi = GET_COMPONENT_FROM(pOwner, CNavigation);
    /*if (pNavi->Is_OnWall())
    {
        _float4 vDir = pPhyscisCom->Get_Physics().vDir;
        vDir = vDir.MultiplyNormal(pNavi->Get_CurWallMat());
        pPhyscisCom->Set_Dir(vDir);
        return;
    }*/

}

STATE_TYPE CLand_Enemy::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);
}

void CLand_Enemy::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();
    pPhyscisCom->Get_PhysicsDetail().fFrictionRatio = 1.f;

}

STATE_TYPE CLand_Enemy::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Run으로 오는 조건
    1. 키눌렸을때
    */
    if (!pOwner->Get_PhysicsCom()->Get_Physics().bAir)
        return m_eStateType;


    return STATE_END;
}
