#include "stdafx.h"
#include "CUnit_Enemy.h"

#include"GameInstance.h"
#include "CUser.h"

#include "Collider.h"

#include "Transform.h"
#include "Physics.h"

#include "CHit_Ground.h"
#include "CBlasted_Ground.h"
#include "CState_Manager.h"
#include "CUnit_Player.h"
#include "CState_Attack.h"
#include "CGuardHit_Enemy.h"

#include "CEnemyHPBar.h"
#include "CEffects_Factory.h"

#include "CBossAura.h"

#include "Functor.h"

CUnit_Enemy::CUnit_Enemy()
{
}

CUnit_Enemy::~CUnit_Enemy()
{
}

CUnit_Enemy* CUnit_Enemy::Create(UNIT_MODEL_DATA& tModelData, CCell* pStartCell)
{
    CUnit_Enemy* pInstance = new CUnit_Enemy();

    if (FAILED(pInstance->SetUp_Model(tModelData)))
    {
        Call_MsgBox(L"Failed to SetUp_Model : CUnit_Enemy");
        SAFE_DELETE(pInstance);
        return nullptr;
    }

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Call_MsgBox(L"Failed to Initialize_Prototype : CUnit_Enemy");
        SAFE_DELETE(pInstance);
        return nullptr;
    }

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CUnit_Enemy");
        SAFE_DELETE(pInstance);
        return nullptr;
    }

    if (FAILED(pInstance->SetUp_Navigation(pStartCell)))
    {
        Call_MsgBox(L"Failed to SetUp_Model : CUnit_Enemy");
        SAFE_DELETE(pInstance);
        return nullptr;
    }

    return pInstance;
}

void CUnit_Enemy::Unit_CollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos)
{
    //플레이어한테 맞음
    if (eColType == COL_PLAYERATTACK)
    {
        __super::Unit_CollisionEnter(pOtherObj, eColType, vColPos);

        //STATE_TYPE ePlayerState = CUser::Get_Instance()->Get_Player()->Get_CurState();
        STATE_TYPE eHitState = STATE_HIT_GROUND_ENEMY;

        //가드상태면 가드로
        if (m_eCurState == STATE_GUARD_ENEMY || m_eCurState == STATE_GUARD_ENEMY)
        {
            _float4 vLook = pOtherObj->Get_Transform()->Get_World(WORLD_POS) - m_pTransform->Get_World(WORLD_POS);
            vLook.y = 0.f;
            GET_STATE(STATE_GUARDHIT_ENEMY, CGuardHit_Enemy)->On_GuardHit(vLook.Normalize(), CGuardHit_Enemy::DEFAULT);
            Enter_State(STATE_GUARDHIT_ENEMY);
        }
        else
        {
            if (Can_Use(DODGE))
            {
                _uint iRand = random(0, 8);

                if (iRand == 3)
                {
                    On_Use(DODGE);
                    Enter_State(STATE_DODGE_ENEMY);
                    if (!PLAYER->Is_LockedTarget())
                    {
                        PLAYER->Set_TargetUnit(nullptr);
                        CUser::Get_Instance()->Set_Target(nullptr);
                    }
                    

                    return;
                }


            }


            #ifdef _DEBUG
                CState_Attack* pState = dynamic_cast<CState_Attack*>(PLAYER->Get_CurStateP());
            if (!pState)
            {
                CState* pState = PLAYER->Get_CurStateP();
                return;
            }
            eHitState = pState->Get_HitState();
            #else
                eHitState = static_cast<CState_Attack*>(PLAYER->Get_CurStateP())->Get_HitState();
            #endif

            Enter_State(eHitState);

            CUser::Get_Instance()->On_RefreshCombo();

            if (PLAYER->Is_NinzaSword())
            {
              CEffects_Factory::Get_Instance()->Create_MultiEffects(L"NSHit", this, m_pTransform->Get_World(WORLD_POS));
            }
            else
            {
                if (eHitState == STATE_BLASTED_GROUND_ENEMY)
                    CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HandHitFinish", this, m_pTransform->Get_World(WORLD_POS));
                else
                    CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HandHit", this, m_pTransform->Get_World(WORLD_POS));
            }

        }
    }


    if (eColType == COL_PLAYERTHROW)
    {
        __super::Unit_CollisionEnter(pOtherObj, eColType, vColPos);
        STATE_TYPE ePlayerState = CUser::Get_Instance()->Get_Player()->Get_CurState();
        STATE_TYPE eHitState = STATE_HIT_GROUND_ENEMY;

        //가드상태면 가드로
        if (m_eCurState == STATE_GUARD_ENEMY || m_eCurState == STATE_GUARD_ENEMY)
        {
            _float4 vLook = pOtherObj->Get_Transform()->Get_World(WORLD_POS) - m_pTransform->Get_World(WORLD_POS);
            vLook.y = 0.f;
            GET_STATE(STATE_GUARDHIT_ENEMY, CGuardHit_Enemy)->On_GuardHit(vLook.Normalize(), CGuardHit_Enemy::DEFAULT);
            Enter_State(STATE_GUARDHIT_ENEMY);
        }
        else
        {
            _float4 vLook = pOtherObj->Get_Transform()->Get_World(WORLD_POS) - m_pTransform->Get_World(WORLD_POS);
            vLook.y = 0.f;
            vLook.Normalize();

            if (vLook.Is_Zero())
            {
                vLook = m_pTransform->Get_World(WORLD_LOOK);
            }

            //풍마수리검 여부
            if (GET_COMPONENT_FROM(pOtherObj, CPhysics)->Get_Physics().fTurnSpeed > 1.f)
            {
                eHitState = STATE_BLASTED_GROUND_ENEMY;
                static_cast<CBlasted_Ground*>(CState_Manager::Get_Instance()->Get_State(STATE_BLASTED_GROUND_ENEMY))->On_BlastedGround(
                    vLook, vLook * -1.f, 10.f, 5.f, 10.f
                );
            }
            else
            {
                CFunctor::Play_Sound(L"Eff_Hit_Kunai", CHANNEL_EFFECTS, m_pTransform->Get_World(WORLD_POS));

                static_cast<CHit_Ground*>(CState_Manager::Get_Instance()->Get_State(STATE_HIT_GROUND_ENEMY))->On_HitGroundDefault(
                    m_pTransform->Get_World(WORLD_LOOK), vLook * -1.f
                );
            }


            CEffects_Factory::Get_Instance()->Create_MultiEffects(L"NSHit", this, m_pTransform->Get_World(WORLD_POS));

            Enter_State(eHitState);
            CUser::Get_Instance()->On_RefreshCombo();

        }
    }


    if (eColType == COL_PLAYERSKILL)
    {
        __super::Unit_CollisionEnter(pOtherObj, eColType, vColPos);
        STATE_TYPE eHitState = STATE_HIT_GROUND_ENEMY;

        //가드상태면 가드로
        if (m_eCurState == STATE_GUARD_ENEMY || m_eCurState == STATE_GUARD_ENEMY)
        {
            _float4 vLook = pOtherObj->Get_Transform()->Get_World(WORLD_POS) - m_pTransform->Get_World(WORLD_POS);
            vLook.y = 0.f;
            GET_STATE(STATE_GUARDHIT_ENEMY, CGuardHit_Enemy)->On_GuardHit(vLook.Normalize(), CGuardHit_Enemy::DEFAULT);
            Enter_State(STATE_GUARDHIT_ENEMY);
        }
        else
        {
            _float4 vLook = pOtherObj->Get_Transform()->Get_World(WORLD_POS) - m_pTransform->Get_World(WORLD_POS);
            vLook.y = 0.f;
            vLook.Normalize();

            if (vLook.Is_Zero())
            {
                vLook = m_pTransform->Get_World(WORLD_LOOK);
            }

            static_cast<CHit_Ground*>(CState_Manager::Get_Instance()->Get_State(STATE_HIT_GROUND_ENEMY))->On_HitGround(
               vLook , vLook * -1.f);


            CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HandHitFinish", this, m_pTransform->Get_World(WORLD_POS));

            Enter_State(eHitState);
            CUser::Get_Instance()->On_RefreshCombo();
        }
    }

}

void CUnit_Enemy::Unit_CollisionStay(CGameObject* pOtherObj, const _uint& eColType)
{
    if (eColType == COL_BODY)
    {
        _float4 vCurPos = m_pTransform->Get_World(WORLD_POS);
        _float4 vDir = vCurPos - pOtherObj->Get_Transform()->Get_World(WORLD_POS);
        vCurPos += vDir.Normalize() * GET_COMPONENT_FROM(pOtherObj, CPhysics)->Get_Physics().fSpeed * fDT;
        m_pTransform->Set_World(WORLD_POS, vCurPos);
        m_pTransform->Make_WorldMatrix();
    }

    __super::Unit_CollisionStay(pOtherObj, eColType);


}

void CUnit_Enemy::On_StageBegin()
{
    ENABLE_GAMEOBJECT(m_pHpBar);

    if (m_tUnitStatus.fMaxHP > 200.f)
    {
        //BOss
        m_fCoolTime[SKILL2] = frandom(20.f, 25.f);

        m_fCoolTime[THROW] = frandom(15.f, 20.f);
    }

    if (m_eSkillType == LAUGHINGMONK)
    {
        m_fCoolTime[SKILL1] = frandom(20.f, 25.f);
        m_fCoolAcc[SKILL1] = frandom(10.f, 15.f);

    }
    else if (m_eSkillType == DOMEDWALL)
    {
        m_fCoolTime[SKILL1] = frandom(15.f, 20.f);
        m_fCoolAcc[SKILL1] = frandom(5.f, 10.f);
    }
    else
    {
        m_fCoolTime[SKILL1] = frandom(10.f, 15.f);
        m_fCoolAcc[SKILL1] = frandom(5.f, 10.f);
    }

}

HRESULT CUnit_Enemy::Initialize()
{
    __super::Initialize();


    m_pColliders[BODY]->Set_ColIndex(COL_BODY);
    m_pColliders[REALBODY]->Set_ColIndex(COL_REALBODY);
    m_pColliders[HITBOX]->Set_ColIndex(COL_ENEMYHITBOX);
    m_pColliders[RIGHTHAND]->Set_ColIndex(COL_ENEMYATTACK);
    m_pColliders[RIGHTFOOT]->Set_ColIndex(COL_ENEMYATTACK);
    m_pColliders[LEFTHAND]->Set_ColIndex(COL_ENEMYATTACK);
    m_pColliders[LEFTFOOT]->Set_ColIndex(COL_ENEMYATTACK);

    m_pHpBar = CEnemyHPBar::Create(this);

    if (FAILED(m_pHpBar->Initialize()))
        return E_FAIL;

    m_fCoolTime[DODGE] = frandom(15.f, 20.f);
    m_fCoolTime[THROW] = frandom(5.f, 10.f);

    m_fCoolTime[SKILL1] = frandom(10.f, 20.f); // LaughingMonk or Mud Wall or Palm Rotation
    m_fCoolTime[SKILL2] = frandom(2.f, 10.f); // Palm Rotation

    m_fCoolAcc[THROW] = frandom(10.f, 20.f);
   // m_fCoolAcc[SKILL1] = frandom(20.f, 40.f);
    m_fCoolAcc[SKILL1] = frandom(0.f, 10.f);

    _uint iRand = random(0, 2);

    m_eSkillType = (SKILL_TYPE)iRand;



   


    

    return S_OK;
}

HRESULT CUnit_Enemy::Start()
{
    __super::Start();

    if (!m_pHpBar)
        return E_FAIL;

    CREATE_GAMEOBJECT(m_pHpBar, GROUP_UI);
    DISABLE_GAMEOBJECT(m_pHpBar);

    m_pTargetUnit = PLAYER;

    m_pTransform->Set_Look(_float4(0.f, 0.f, -1.f, 0.f));



    return S_OK;
}

void CUnit_Enemy::My_Tick()
{
    __super::My_Tick();

    for (_uint i = 0; i < COOL_END; ++i)
    {
        if (m_fCoolAcc[i] > 0.f)
        {
            m_fCoolAcc[i] -= fDT;

            if (m_fCoolAcc[i] <= 0.f)
            {
                m_fCoolAcc[i] = 0.f;
            }
        }
    }
   
}

void CUnit_Enemy::OnEnable()
{
    __super::OnEnable();
    ENABLE_GAMEOBJECT(m_pHpBar);
    //Enter_State(STATE_IDLE_ENEMY);

}

void CUnit_Enemy::OnDisable()
{
    __super::OnDisable();
    DISABLE_GAMEOBJECT(m_pHpBar);
}

