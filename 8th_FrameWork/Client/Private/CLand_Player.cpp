#include "stdafx.h"
#include "CLand_Player.h"

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
#include "Functor.h"
#include "CCell.h"

CLand_Player::CLand_Player()
{
}

CLand_Player::~CLand_Player()
{
}

CLand_Player* CLand_Player::Create()
{
    CLand_Player* pInstance = new CLand_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CLand_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CLand_Player::Initialize()
{
    m_iAnimIndex = 37;
    m_eStateType = STATE_LAND_PLAYER;

    m_vecAdjState.push_back(STATE_IDLE_PLAYER);
    m_vecAdjState.push_back(STATE_CJ_START_PLAYER);
    m_vecAdjState.push_back(STATE_DASH_PLAYER);
    m_vecAdjState.push_back(STATE_FALL_PLAYER);
    m_vecAdjState.push_back(STATE_JUMP_PLAYER);
    m_vecAdjState.push_back(STATE_NSATTACK_1_PLAYER);
    m_vecAdjState.push_back(STATE_HANDATTACK_1_PLAYER);
    m_vecAdjState.push_back(STATE_HANDATTACK_POWER_PLAYER);
    m_vecAdjState.push_back(STATE_GUARD_PLAYER);
    m_vecAdjState.push_back(STATE_THROW_PLAYER);
    m_vecAdjState.push_back(STATE_RASENSURIKEN_PLAYER);
    m_vecAdjState.push_back(STATE_RASENGAN);
    m_vecAdjState.push_back(STATE_KAMUI_PLAYER);
    m_vecAdjState.push_back(STATE_EIGHTGATES);
    m_vecAdjState.push_back(STATE_CHIDORI);
    m_vecAdjState.push_back(STATE_FIREBALL_PLAYER);

    m_vecAdjState.push_back(STATE_WATERPRISON_PLAYER);
    m_vecAdjState.push_back(STATE_SHARKBOMB_PLAYER);
    m_vecAdjState.push_back(STATE_WATERFALL_PLAYER);

    m_fInterPolationTime = 0.05f;
    m_fStateChangableTime = 0.2f;
    m_fDelayTime = 0.55f;

    return S_OK;
}

void CLand_Player::Enter(CUnit* pOwner, CAnimator* pAnimator)
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

    if (pPhyscisCom->Get_Physics().fAcc >= 3.f || pPhyscisCom->Get_Physics().fSpeed > pOwner->Get_Status().fRunSpeed)
    {
        m_iAnimIndex = 35;
        m_fAnimSpeed = 1.3f;
        m_fStateChangableTime = 0.35f;

        pPhyscisCom->Set_MaxSpeed(pOwner->Get_Status().fShortDashSpeed);
        pPhyscisCom->Set_Speed(pOwner->Get_Status().fShortDashSpeed);
        pPhyscisCom->Get_PhysicsDetail().fFrictionRatio = 0.5f;



    }


    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator);
    CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_DEFAULT);

    CState_Manager::Get_Instance()->Get_State(STATE_DOUBLEJUMP_PLAYER)->ReSet_CoolTime();
    CState_Manager::Get_Instance()->Get_State(STATE_AIRDASHSTART_PLAYER)->ReSet_CoolTime();

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

STATE_TYPE CLand_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);
}

void CLand_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();
    pPhyscisCom->Get_PhysicsDetail().fFrictionRatio = 1.f;

}

STATE_TYPE CLand_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Run으로 오는 조건
    1. 키눌렸을때
    */
    if (!pOwner->Get_PhysicsCom()->Get_Physics().bAir)
        return m_eStateType;


    return STATE_END;
}
