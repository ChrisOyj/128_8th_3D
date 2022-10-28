#include "stdafx.h"
#include "CDown_Enemy.h"

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
CDown_Enemy::CDown_Enemy()
{
}

CDown_Enemy::~CDown_Enemy()
{
}

CDown_Enemy* CDown_Enemy::Create()
{
    CDown_Enemy* pInstance = new CDown_Enemy();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CDown_Enemy");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CDown_Enemy::Initialize()
{
    m_eAnimType = ANIM_BEATEN;
    m_iAnimIndex = 42;
    m_eStateType = STATE_DOWN_ENEMY;

    m_vecAdjState.push_back(STATE_GETUP_ENEMY);

    m_fInterPolationTime = 0.05f;
    m_fStateChangableTime = 0.5f;

    return S_OK;
}

void CDown_Enemy::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    CFunctor::Play_Sound(L"Env_LandHard", CHANNEL_ENVIRONMENT, pOwner->Get_Transform()->Get_World(WORLD_POS));

    CEffects_Factory::Get_Instance()->Create_LandingEffects(pOwner->Get_Transform()->Get_World(WORLD_POS), GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());
    CGameObject* pGroundBreak = CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"GroundBreak_A"), pOwner->Get_Transform()->Get_World(WORLD_POS));
    pGroundBreak->Get_Transform()->Set_Up(_float4(0.f, 1.f, 0.f, 0.f));
    CEffects_Factory::Get_Instance()->Create_MultiEffects(L"GroundSmoke", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));

    if (m_bFront)
        m_iAnimIndex = 43;
    else
        m_iAnimIndex = 42;

    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();
    pPhyscisCom->Get_PhysicsDetail().fFrictionRatio = 1.f;
    pPhyscisCom->Set_Speed(0.f);

    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator);

}

STATE_TYPE CDown_Enemy::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);
}

void CDown_Enemy::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */

}

STATE_TYPE CDown_Enemy::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Run으로 오는 조건
    1. 키눌렸을때
    */
    if (!pOwner->Get_PhysicsCom()->Get_Physics().bAir)
        return m_eStateType;


    return STATE_END;
}
