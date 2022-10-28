#include "stdafx.h"
#include "CThrow_Enemy.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "CCamera_Free.h"
#include "Physics.h"

#include "CUser.h"

#include "CScript_FollowCam.h"
#include "CEffects_Factory.h"
#include "CCollider_Sphere.h"
#include "CKunai.h"
#include "CNavigation.h"

#include "CBossKunai.h"

#include "Functor.h"

CThrow_Enemy::CThrow_Enemy()
{
}

CThrow_Enemy::~CThrow_Enemy()
{
}

CThrow_Enemy* CThrow_Enemy::Create()
{
    CThrow_Enemy* pInstance = new CThrow_Enemy();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CThrow_Enemy");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CThrow_Enemy::Initialize()
{
    //left 1, right 2, air 3, air windmill 17, default 19, moving windmill 34

    m_eAnimType = ANIM_THROW;
    m_iAnimIndex = 19;
    m_eStateType = STATE_THROW_ENEMY;


    //멀어졌으면 run
    m_vecAdjState.push_back(STATE_RUN_ENEMY);
    m_vecAdjState.push_back(STATE_JUMP_ENEMY);

    //공격범위 안이면 공격, 대쉬
    m_vecAdjState.push_back(STATE_DASH_ENEMY);
    m_vecAdjState.push_back(STATE_GUARD_ENEMY);
    m_vecAdjState.push_back(STATE_IDLE_ENEMY);
    m_vecAdjState.push_back(STATE_HANDATTACK_1_ENEMY);


    m_fInterPolationTime = 0.05f;
    m_fStateChangableTime = 0.7f;
    m_fAnimSpeed = 1.1f;

    return S_OK;
}

void CThrow_Enemy::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    //left 1, right 2, air 3, air windmill 17, default 19, moving windmill 34

    CFunctor::Play_Sound(L"Eff_MoveStrong", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

    pOwner->On_Use(CUnit::THROW);
    m_iAnimIndex = 19;
    m_fDelayTime = 0.4f;

    _float fDashSpeed = pOwner->Get_Status().fShortDashSpeed;

    CTransform* pTransform = pOwner->Get_Transform();
    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();

    CUnit* pPlayer = pOwner->Get_TargetUnit();
    _float4 vTargetLook = pPlayer->Get_Transform()->Get_World(WORLD_POS) - pTransform->Get_World(WORLD_POS);
    vTargetLook.y = 0.f;
    vTargetLook.Normalize();

    _float4 _vLook = vTargetLook;
    _float4 vUp = _float4(0.f, 1.f, 0.f, 0.f);
    _float4 vRight = vUp.Cross(_vLook);
    _vLook = vRight.Cross(vUp.Normalize());
    _vLook.Normalize();

    pTransform->Set_LerpLook(_vLook, 0.4f);



    _float4 vDir = { 0.f, 0.f, 0.f, 0.f };
    _int iRand = random(0, 2);

    if (pPhyscisCom->Get_Physics().bAir)
    {
        m_fDelayTime = 0.25f;
        m_fStateChangableTime = 0.5f;
        m_iAnimIndex = 3;
    }
    else if (iRand == 0)
    {
        m_fDelayTime = 0.25f;
        m_fStateChangableTime = 0.6f;
        vDir -= vRight;
        m_iAnimIndex = 1;
    }
    else if (iRand == 1)
    {
        m_fDelayTime = 0.25f;
        m_fStateChangableTime = 0.6f;
        vDir += vRight;
        m_iAnimIndex = 2;
    }

    //안눌렸으면 이동 x
    if (vDir.Is_Zero())
    {
        __super::Enter(pOwner, pAnimator);
        return;
    }

    pPhyscisCom->Set_MaxSpeed(fDashSpeed);
    pPhyscisCom->Set_Speed(fDashSpeed);
    pPhyscisCom->Set_Jump(3.f);
    pPhyscisCom->Set_Dir(vDir.Normalize());
    pPhyscisCom->Get_PhysicsDetail().fFrictionRatio = 0.5f;

    __super::Enter(pOwner, pAnimator);

}

STATE_TYPE CThrow_Enemy::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_fStateChangableTime <= m_fTimeAcc + fDT &&
        pOwner->Get_PhysicsCom()->Get_Physics().bAir)
    {
        return STATE_FALL_ENEMY;
    }

    return __super::Tick(pOwner, pAnimator);
}

void CThrow_Enemy::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;

    if (pOwner->Get_PhysicsCom()->Get_Physics().bAir)
        pOwner->Get_PhysicsCom()->Set_Speed(pOwner->Get_PhysicsCom()->Get_Physics().fSpeed * 0.7f);

}

STATE_TYPE CThrow_Enemy::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    _float4 vCurPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
    _float4 vTargetPos = pOwner->Get_TargetUnit()->Get_Transform()->Get_World(WORLD_POS);
    vTargetPos.y = 0.f;
    vCurPos.y = 0.f;
    _float4 vDir = vTargetPos - vCurPos;
    _float fTargetLength = vDir.Length();

    if (fTargetLength < 20.f &&
        fTargetLength > 4.f &&
        pOwner->Can_Use(CUnit::THROW))
    {
        return m_eStateType;
    }


    return STATE_END;
}

void CThrow_Enemy::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
    CFunctor::Play_Sound(L"Eff_Throw", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

    CEffects_Factory::Get_Instance()->Create_LandingEffects(pOwner->Get_Transform()->Get_World(WORLD_POS), GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());

    CTransform* pTransform = pOwner->Get_Transform();
    _float4 vMyPos = pTransform->Get_World(WORLD_POS);
    vMyPos.y += 0.6f;
    _float4 vLook = pTransform->Get_World(WORLD_LOOK);
    _float4 vTargetPos;
    if (m_iAnimIndex == 3)
    {
        pOwner->Get_PhysicsCom()->Set_Jump(5.f);
    }
    if (pOwner->Get_TargetUnit())
    {
        //타겟이 있으면 타겟쪽으로 수리검 생성
        vTargetPos = pOwner->Get_TargetUnit()->Get_Transform()->Get_World(WORLD_POS);
        vTargetPos.y += 0.6f;

        //아니면 바라보는 곳으로 걍 생성
    }
    else
    {
        vTargetPos = vMyPos + vLook * 30.f;
    }

    if (pOwner->Get_Status().fMaxHP > 200.f)
    {
        CGameObject* pGameObejct = CEffects_Factory::Get_Instance()->Create_Effects(HASHCODE(CBossKunai), vMyPos, vTargetPos);
        static_cast<CBossKunai*>(pGameObejct)->Set_ColType(COL_ENEMYSKILL);
        static_cast<CEffect*>(pGameObejct)->Set_FollowTarget(pOwner->Get_TargetUnit());
        GET_COMPONENT_FROM(pGameObejct, CNavigation)->Set_StartCell(GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());

    }
    else
    {

        CGameObject* pGameObejct = CEffects_Factory::Get_Instance()->Create_Effects(HASHCODE(CKunai), vMyPos, vTargetPos);
        GET_COMPONENT_FROM(pGameObejct, CCollider_Sphere)->Set_ColIndex(COL_ENEMYTHROW);
        GET_COMPONENT_FROM(pGameObejct, CNavigation)->Set_StartCell(GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());

    }
    
}
