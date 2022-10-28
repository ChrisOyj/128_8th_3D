#include "stdafx.h"
#include "CHit_Air_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CScript_FollowCam.h"

#include "Transform.h"
#include "Physics.h"
#include "Functor.h"
CHit_Air_Player::CHit_Air_Player()
{
}

CHit_Air_Player::~CHit_Air_Player()
{
}

CHit_Air_Player* CHit_Air_Player::Create()
{
    CHit_Air_Player* pInstance = new CHit_Air_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CHit_Air_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

void CHit_Air_Player::On_HitAir(HIT_TYPE eType, _float4 vLook, _float4 vDir, _float fSpeed, _float fJumpPower, _float fDamage)
{
    m_eHitType = eType;
    m_vLook = vLook;
    m_vDir = vDir;
    m_fSpeed = fSpeed;
    m_fJumpPower = fJumpPower;
    m_fDamage = fDamage;
}

HRESULT CHit_Air_Player::Initialize()
{
    m_eAnimType = ANIM_BEATEN;
    m_iAnimIndex = 58;
    m_eStateType = STATE_HIT_AIR_PLAYER;
    m_fInterPolationTime = 0.05f;

    m_vecAdjState.push_back(STATE_FALL_PLAYER);
    m_vecAdjState.push_back(STATE_LAND_PLAYER);


    return S_OK;
}

void CHit_Air_Player::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    CFunctor::Play_Sound(L"Eff_Hit_Hand", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

    CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_BACKDASH);

    switch (m_eHitType)
    {
    case Client::CHit_Air_Player::SHORT:
        m_iAnimIndex = 9;
        break;
    case Client::CHit_Air_Player::LONG:
        m_iAnimIndex = 7;
        break;
    case Client::CHit_Air_Player::GOUP:
        m_iAnimIndex = 61;
        break;

    case Client::CHit_Air_Player::HIT_END:
        break;
    default:
        break;
    }

    CTransform* pTransformCom = pOwner->Get_Transform();
    CPhysics* pPhysicsCom = pOwner->Get_PhysicsCom();

    pTransformCom->Set_LerpLook(m_vLook, 0.4f);
    pPhysicsCom->Set_Dir(m_vDir);
    pPhysicsCom->Set_MaxSpeed(m_fSpeed);
    pPhysicsCom->Set_Speed(m_fSpeed);
    pPhysicsCom->Set_Jump(m_fJumpPower);



    __super::Enter(pOwner, pAnimator);
    pOwner->On_PlusHp(-m_fDamage);
}

STATE_TYPE CHit_Air_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (CUser::Get_Instance()->Get_LastKey() == KEY::E &&
        CUser::Get_Instance()->Can_UseSkill(0))
    {
        return STATE_DODGE_PLAYER;
    }
    return __super::Tick(pOwner, pAnimator);
}

void CHit_Air_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CHit_Air_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{


    return STATE_END;
}
