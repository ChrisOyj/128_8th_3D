#include "stdafx.h"
#include "CWallHit_Enemy.h"

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

CWallHit_Enemy::CWallHit_Enemy()
{
}

CWallHit_Enemy::~CWallHit_Enemy()
{
}

CWallHit_Enemy* CWallHit_Enemy::Create()
{
    CWallHit_Enemy* pInstance = new CWallHit_Enemy();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWallHit_Enemy");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CWallHit_Enemy::Initialize()
{
    m_eAnimType = ANIM_BEATEN;
    m_iAnimIndex = 44;
    m_eStateType = STATE_WALLHIT_ENEMY;

    m_vecAdjState.push_back(STATE_WALLHITEND_ENEMY);

    m_fInterPolationTime = 0.05f;

    return S_OK;
}

void CWallHit_Enemy::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    CFunctor::Play_Sound(L"Env_LandHard", CHANNEL_ENVIRONMENT, pOwner->Get_Transform()->Get_World(WORLD_POS));

    _float4 vPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
    vPos.y += 0.2f;
    CGameObject* pBreak = CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"GroundBreak_A"), vPos);

    CEffects_Factory::Get_Instance()->Create_MultiEffects(L"WallDust", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));

    _float4 vLook = GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurWallNormal();
    pBreak->Get_Transform()->Set_Up(vLook);
    pBreak->Get_Transform()->Make_WorldMatrix();

    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();
    pPhyscisCom->Get_PhysicsDetail().fFrictionRatio = 1.f;
    pPhyscisCom->Set_Speed(0.f);
    pPhyscisCom->Set_Jump(0.f);
    pPhyscisCom->Get_Physics().fGravity = 0.f;

    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator);

}

STATE_TYPE CWallHit_Enemy::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);
}

void CWallHit_Enemy::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
    pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 9.8f;

}

STATE_TYPE CWallHit_Enemy::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Run으로 오는 조건
    1. 키눌렸을때
    */
    if (GET_COMPONENT_FROM(pOwner, CNavigation)->Is_OnWall())
        return m_eStateType;


    return STATE_END;
}
