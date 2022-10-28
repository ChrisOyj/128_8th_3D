#include "stdafx.h"
#include "CBlasted_Ground_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CScript_FollowCam.h"

#include "Transform.h"
#include "Physics.h"

#include "CEffects_Factory.h"
#include "CScreenEffect.h"
#include "Functor.h"

CBlasted_Ground_Player::CBlasted_Ground_Player()
{
}

CBlasted_Ground_Player::~CBlasted_Ground_Player()
{
}

CBlasted_Ground_Player* CBlasted_Ground_Player::Create()
{
    CBlasted_Ground_Player* pInstance = new CBlasted_Ground_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CBlasted_Ground_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
void CBlasted_Ground_Player::On_BlastedGround(_float4 vLook, _float4 vDir, _float fSpeed, _float fJumpPower, _float fDamage)
{
    m_vLook = vLook;
    m_vDir = vDir;
    m_fSpeed = fSpeed;
    m_fJumpPower = fJumpPower;
    m_fDamage = fDamage;
}

HRESULT CBlasted_Ground_Player::Initialize()
{
    m_eAnimType = ANIM_BEATEN;
    m_iAnimIndex = 25;
    m_eStateType = STATE_BLASTED_GROUND_PLAYER;
    m_fInterPolationTime = 0.05f;
    m_fStateChangableTime = 0.5f;


    m_vecAdjState.push_back(STATE_IDLE_NIGHTGUY);
    m_vecAdjState.push_back(STATE_DASH_NIGHTGUY);


    m_vecAdjState.push_back(STATE_IDLE_PLAYER);
    m_vecAdjState.push_back(STATE_DASH_PLAYER);
    m_vecAdjState.push_back(STATE_WALLHIT_PLAYER);

    return S_OK;
}

void CBlasted_Ground_Player::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    CFunctor::Play_Sound(L"Eff_KO_Hand", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
    CFunctor::Play_Sound(L"Voice_Man_Hit", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));
    CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_BACKDASH);
    CEffects_Factory::Get_Instance()->Create_ScreenEffects(pOwner);


    CTransform* pTransformCom = pOwner->Get_Transform();
    CPhysics* pPhysicsCom = pOwner->Get_PhysicsCom();

    pTransformCom->Set_LerpLook(m_vLook, 0.3f);
    pPhysicsCom->Set_Dir(m_vDir);
    pPhysicsCom->Set_MaxSpeed(m_fSpeed);
    pPhysicsCom->Set_Speed(m_fSpeed);
    pPhysicsCom->Set_Jump(m_fJumpPower);


    __super::Enter(pOwner, pAnimator);
    pOwner->On_PlusHp(-m_fDamage);
}

STATE_TYPE CBlasted_Ground_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (CUser::Get_Instance()->Get_LastKey() == KEY::E &&
        CUser::Get_Instance()->Can_UseSkill(0))
    {
        if (!pOwner->Is_NightGuy())
            return STATE_DODGE_PLAYER;
    }

    return __super::Tick(pOwner, pAnimator);
}

void CBlasted_Ground_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_DEFAULT);
}

STATE_TYPE CBlasted_Ground_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{


    return STATE_END;
}
