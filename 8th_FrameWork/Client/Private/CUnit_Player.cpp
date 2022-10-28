#include "stdafx.h"
#include "CUnit_Player.h"

#include"GameInstance.h"
#include "CUser.h"

#include "CCollider_Sphere.h"
#include "Physics.h"
#include "CHit_Ground.h"
#include "CState_Manager.h"

#include "Transform.h"
#include "CState_Attack_Enemy.h"
#include "CGuardHit_Player.h"
#include "CScript_FollowCam.h"
#include "CPose.h"
#include "Texture.h"
#include "Model.h"

#include "CTrailEffect.h"
#include "CEffects_Factory.h"
#include "CTrailBuffer.h"

#include "CHit_Ground_Player.h"

#include "Functor.h"

CUnit_Player::CUnit_Player()
{
}

CUnit_Player::~CUnit_Player()
{
}

CUnit_Player* CUnit_Player::Create(UNIT_MODEL_DATA& tModelData, CCell* pStartCell)
{
    CUnit_Player* pInstance = new CUnit_Player();
    
    if (FAILED(pInstance->SetUp_Model(tModelData)))
    {
        Call_MsgBox(L"Failed to SetUp_Model : CUnit_Player");
        SAFE_DELETE(pInstance);
        return nullptr;
    }

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Call_MsgBox(L"Failed to Initialize_Prototype : CUnit_Player");
        SAFE_DELETE(pInstance);
        return nullptr;
    }

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CUnit_Player");
        SAFE_DELETE(pInstance);
        return nullptr;
    }

    if (FAILED(pInstance->SetUp_Navigation(pStartCell)))
    {
        Call_MsgBox(L"Failed to SetUp_Model : CUnit_Player");
        SAFE_DELETE(pInstance);
        return nullptr;
    }

    return pInstance;
}

void CUnit_Player::On_FixedTarget(_bool bActivate)
{
    m_bFixedTarget = bActivate;
    CUser::Get_Instance()->On_FixedTarget(bActivate);
}

void CUnit_Player::On_PlusHp(_float fHp)
{
    __super::On_PlusHp(fHp);

    CUser::Get_Instance()->Start_HpLerp(m_tUnitStatus.fHP, m_tUnitStatus.fMaxHP);
}

void CUnit_Player::On_Die()
{
    //__super::On_Die();
}

void CUnit_Player::TurnOn_TrailBuffer()
{
        static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_TrailEffects.front(), CMesh))->Set_TrailOn();
}

void CUnit_Player::TurnOff_TrailBuffer()
{
        static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(m_TrailEffects.front(), CMesh))->Set_TrailOff();
}
void CUnit_Player::TurnOn_AllTrailBuffer()
{
    for (auto& elem : m_TrailEffects)
        if (elem != m_TrailEffects.front())
        static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(elem, CMesh))->Set_TrailOn();
}
void CUnit_Player::TurnOff_AllTrailBuffer()
{
    for (auto& elem : m_TrailEffects)
        if (elem != m_TrailEffects.front())
            static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(elem, CMesh))->Set_TrailOff();
}
void CUnit_Player::Unit_CollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos)
{
    if (eColType == COL_ENEMYHITBOX)
    {
        CUser::Get_Instance()->Get_FollowCam()->Start_ShakingCamera(0.03f, 0.2f);

        m_pPhysics->Mul_Speed(0.5f);
    }

    //적한테 맞음
    if (eColType == COL_ENEMYATTACK)
    {
        CUser::Get_Instance()->Disable_Wire();
        CUser::Get_Instance()->Get_FollowCam()->Start_ShakingCamera(0.03f, 0.2f);

        __super::Unit_CollisionEnter(pOtherObj, eColType, vColPos);

        STATE_TYPE eEnemyState = static_cast<CUnit*>(pOtherObj)->Get_CurState();
        STATE_TYPE eHitState = STATE_HIT_GROUND_PLAYER;

        //가드상태면 가드로
        if (m_eCurState == STATE_GUARD_PLAYER || m_eCurState == STATE_GUARDHIT_PLAYER)
        {
            _float4 vLook = pOtherObj->Get_Transform()->Get_World(WORLD_POS) - m_pTransform->Get_World(WORLD_POS);
            vLook.y = 0.f;

            GET_STATE(STATE_GUARDHIT_PLAYER, CGuardHit_Player)->On_GuardHit(vLook.Normalize(), CGuardHit_Player::DEFAULT);
            Enter_State(STATE_GUARDHIT_PLAYER);
        }
        else //if (m_eCurState < STATE_HANDATTACK_1_PLAYER)
        {
#ifdef _DEBUG
            CState_Attack_Enemy* pState = dynamic_cast<CState_Attack_Enemy*>(CState_Manager::Get_Instance()->Get_State(eEnemyState));
            if (!pState)
            {
                /*CState* pState = static_cast<CUnit*>(pOtherObj)->Get_CurStateP();
                assert(0);
                Call_MsgBox(L"State Error... CUnit_Player");*/
                return;
            }
            eHitState = pState->Get_HitState();
#else
            eHitState = static_cast<CState_Attack_Enemy*>(CState_Manager::Get_Instance()->Get_State(eEnemyState))->Get_HitState();
#endif

            Enter_State(eHitState);

            if (eHitState == STATE_BLASTED_GROUND_PLAYER)
                CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HandHitFinish", this, m_pTransform->Get_World(WORLD_POS));
            else
                CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HandHit", this, m_pTransform->Get_World(WORLD_POS));
        }

    }

    if (eColType == COL_ENEMYTHROW)
    {

        CUser::Get_Instance()->Get_FollowCam()->Start_ShakingCamera(0.03f, 0.2f);
        __super::Unit_CollisionEnter(pOtherObj, eColType, vColPos);

        if (m_eCurState == STATE_GUARD_PLAYER || m_eCurState == STATE_GUARDHIT_PLAYER)
        {
            CFunctor::Play_Sound(L"Eff_Hit_Sword", CHANNEL_EFFECTS, m_pTransform->Get_World(WORLD_POS));

            _float4 vLook = pOtherObj->Get_Transform()->Get_World(WORLD_POS) - m_pTransform->Get_World(WORLD_POS);
            vLook.y = 0.f;
            GET_STATE(STATE_GUARDHIT_PLAYER, CGuardHit_Player)->On_GuardHit(vLook.Normalize(), CGuardHit_Player::DEFAULT);
            Enter_State(STATE_GUARDHIT_PLAYER);
        }
        else
        {
            CFunctor::Play_Sound(L"Eff_Hit_Kunai", CHANNEL_EFFECTS, m_pTransform->Get_World(WORLD_POS));
            CEffects_Factory::Get_Instance()->Create_MultiEffects(L"NSHit", this, m_pTransform->Get_World(WORLD_POS));
            On_PlusHp(-2.f);

        }
    }


    if (eColType == COL_ENEMYSKILL)
    {
        __super::Unit_CollisionEnter(pOtherObj, eColType, vColPos);
        STATE_TYPE eHitState = STATE_HIT_GROUND_PLAYER;

        //가드상태면 가드로
        if (m_eCurState == STATE_GUARD_PLAYER || m_eCurState == STATE_GUARDHIT_PLAYER)
        {
            _float4 vLook = pOtherObj->Get_Transform()->Get_World(WORLD_POS) - m_pTransform->Get_World(WORLD_POS);
            vLook.y = 0.f;
            GET_STATE(STATE_GUARDHIT_PLAYER, CGuardHit_Player)->On_GuardHit(vLook.Normalize(), CGuardHit_Player::DEFAULT);
            Enter_State(STATE_GUARDHIT_PLAYER);
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

            static_cast<CHit_Ground_Player*>(CState_Manager::Get_Instance()->Get_State(STATE_HIT_GROUND_PLAYER))->On_HitGround(
                vLook, vLook * -1.f);


            CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HandHitFinish", this, m_pTransform->Get_World(WORLD_POS));

            Enter_State(eHitState);
        }
    }

}

void CUnit_Player::Unit_CollisionStay(CGameObject* pOtherObj, const _uint& eColType)
{
    __super::Unit_CollisionStay(pOtherObj, eColType);
}

HRESULT CUnit_Player::Initialize()
{
    __super::Initialize();

    m_tUnitStatus.fMaxHP = 300.f;
    m_tUnitStatus.fHP = m_tUnitStatus.fMaxHP;

    CTexture* pTexture = CTexture::Create(0, L"../bin/resources/textures/Materials/T_noise_Type_%d.dds", 3);
    pTexture->Set_CurTextureIndex(1);
    Add_Component(pTexture);

    
    CTrailEffect* pTrailEffect = CTrailEffect::Create(CP_RIGHTBEFORE_RENDERER, 100, _float4(0.f, 0.f, 0.f, 1.f), _float4(0.f, -80.f, 0.f, 1.f)
        , GET_COMPONENT(CModel)->Find_HierarchyNode("R_Hand_Weapon_cnt_tr"), m_pTransform, _float4(0.3f, 0.3f, 0.3f, 0.0f), _float4(1.f, 1.f, 1.f, 3.f),
        L"../bin/resources/textures/effects/gradientMap/T_EFF_Blur_04_M.dds",
        L"../bin/resources/textures/effects/gradationcolor/T_EFF_GMS_Light_01_BC.png"
       );

    //static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(pTrailEffect, CMesh))->Set_NoCurve();

    if (!pTrailEffect)
        return E_FAIL;

    m_TrailEffects.push_back(pTrailEffect);


    ///* Head */
    //_uint iTrailNum = 10;

    //_float4 vColor = _float4(1.f, 1.f, 1.f, 3.f);
    //
    //
    //pTrailEffect = CTrailEffect::Create(CP_RIGHTBEFORE_RENDERER, iTrailNum, _float4(0.f, 0.f, -20.f, 1.f), _float4(0.f, 0.f, 20.f, 1.f)
    //    , GET_COMPONENT(CModel)->Find_HierarchyNode("Head"), m_pTransform, _float4(0.3f, 0.3f, 0.3f, 0.0f), vColor,
    //    L"../bin/resources/textures/effects/gradientMap/T_EFF_Blur_04_M.dds",
    //    L"../bin/resources/textures/effects/gradationcolor/T_EFF_GMS_Light_01_BC.png"
    //);

    //if (!pTrailEffect)
    //    return E_FAIL;

    //static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(pTrailEffect, CMesh))->Set_NoCurve();

    //m_TrailEffects.push_back(pTrailEffect);


    ///* Spine */

    //wstring wstrBodyPath;

    //if (m_tModelData.bOverall)
    //    wstrBodyPath = L"../bin/resources/textures/effects/gradationcolor/T_EFF_GMS_Light_01_BC.png";
    //else
    //    wstrBodyPath = L"../bin/resources/textures/effects/gradationcolor/T_EFF_GMS_Light_01_BC.png";

    //pTrailEffect = CTrailEffect::Create(CP_RIGHTBEFORE_RENDERER, iTrailNum, _float4(0.f, 0.f, -40.f, 1.f), _float4(0.f, 00.f, 40.f, 1.f)
    //    , GET_COMPONENT(CModel)->Find_HierarchyNode("Spine"), m_pTransform, _float4(0.3f, 0.3f, 0.3f, 0.0f), vColor,
    //    L"../bin/resources/textures/effects/gradientMap/T_EFF_Blur_04_M.dds",
    //    wstrBodyPath
    //);

    //if (!pTrailEffect)
    //    return E_FAIL;
    //static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(pTrailEffect, CMesh))->Set_NoCurve();

    //m_TrailEffects.push_back(pTrailEffect);

    ///* Arm */

    //pTrailEffect = CTrailEffect::Create(CP_RIGHTBEFORE_RENDERER, iTrailNum, _float4(0.f, 0.f, -0.f, 1.f), _float4(40.f, 00.f, 40.f, 1.f)
    //    , GET_COMPONENT(CModel)->Find_HierarchyNode("RightForeArm"), m_pTransform, _float4(0.3f, 0.3f, 0.3f, 0.0f), vColor,
    //    L"../bin/resources/textures/effects/gradientMap/T_EFF_Blur_04_M.dds",
    //    wstrBodyPath
    //);

    //if (!pTrailEffect)
    //    return E_FAIL;
    //static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(pTrailEffect, CMesh))->Set_NoCurve();

    //m_TrailEffects.push_back(pTrailEffect);


    //pTrailEffect = CTrailEffect::Create(CP_RIGHTBEFORE_RENDERER, iTrailNum, _float4(0.f, 0.f, -0.f, 1.f), _float4(-40.f, 00.f, 40.f, 1.f)
    //    , GET_COMPONENT(CModel)->Find_HierarchyNode("LeftForeArm"), m_pTransform, _float4(0.3f, 0.3f, 0.3f, 0.0f), vColor,
    //    L"../bin/resources/textures/effects/gradientMap/T_EFF_Blur_04_M.dds",
    //    wstrBodyPath
    //);

    //if (!pTrailEffect)
    //    return E_FAIL;
    //static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(pTrailEffect, CMesh))->Set_NoCurve();

    //m_TrailEffects.push_back(pTrailEffect);

    ///* Leg */

    //pTrailEffect = CTrailEffect::Create(CP_RIGHTBEFORE_RENDERER, iTrailNum, _float4  (10.f, 0.f, -50.f, 1.f), _float4(40.f, 00.f, 40.f, 1.f)
    //    , GET_COMPONENT(CModel)->Find_HierarchyNode("RightLeg"), m_pTransform, _float4(0.3f, 0.3f, 0.3f, 0.0f), vColor,
    //    L"../bin/resources/textures/effects/gradientMap/T_EFF_Blur_04_M.dds",
    //    wstrBodyPath
    //);

    //if (!pTrailEffect)
    //    return E_FAIL;
    //static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(pTrailEffect, CMesh))->Set_NoCurve();

    //m_TrailEffects.push_back(pTrailEffect);


    //pTrailEffect = CTrailEffect::Create(CP_RIGHTBEFORE_RENDERER, iTrailNum, _float4(-10.f, 0.f, -50.f, 1.f), _float4(0.f, 00.f, 0.f, 1.f)
    //    , GET_COMPONENT(CModel)->Find_HierarchyNode("LeftLeg"), m_pTransform, _float4(0.3f, 0.3f, 0.3f, 0.0f), vColor,
    //    L"../bin/resources/textures/effects/gradientMap/T_EFF_Blur_04_M.dds",
    //    wstrBodyPath
    //);

    //if (!pTrailEffect)
    //    return E_FAIL;
    //static_cast<CTrailBuffer*>(GET_COMPONENT_FROM(pTrailEffect, CMesh))->Set_NoCurve();

    //m_TrailEffects.push_back(pTrailEffect);


    //_uint iIndex = 0;
    //for (auto& elem : m_TrailEffects)
    //{
    //    wstring wstrStaticCode;
    //    wstrStaticCode = L"TrailEffect_";
    //    wstrStaticCode += to_wstring(iIndex++);
    //    elem->Initialize();
    //    CREATE_STATIC(elem, Convert_ToHash(wstrStaticCode));
    //    DISABLE_GAMEOBJECT(elem);
    //}
    


    //m_pTrailEffect2 = CTrailEffect::Create(CP_RIGHTBEFORE_RENDERER, 200, _float4(0.f, 0.f, 0.f, 1.f), _float4(0.f, 0.f, 80.f, 1.f)
    //    , GET_COMPONENT(CModel)->Find_HierarchyNode("RightLeg"), m_pTransform, _float4(0.3f, 0.3f, 0.3f, 0.2f), _float4(1.f, 1.f, 1.f, 3.f),
    //    L"../bin/resources/textures/effects/gradientMap/T_EFF_Blur_04_M.dds",
    //    L"../bin/resources/textures/effects/gradationcolor/T_EFF_GMS_Light_01_BC.png"
    //);

    //if (!m_pTrailEffect2)
    //    return E_FAIL;

    //m_pTrailEffect2->Initialize();
    //CREATE_STATIC(m_pTrailEffect2, HASHCODE(CTrailEffect));
    //DISABLE_GAMEOBJECT(m_pTrailEffect2);


    _uint iIndex = 0;
    for (auto& elem : m_TrailEffects)
    {
        wstring wstrStaticCode;
        wstrStaticCode = L"TrailEffect_";
        wstrStaticCode += to_wstring(iIndex++);
        elem->Initialize();
        CREATE_STATIC(elem, Convert_ToHash(wstrStaticCode));
        DISABLE_GAMEOBJECT(elem);
    }


    m_pColliders[BODY]->Set_ColIndex(COL_BODY);
    m_pColliders[REALBODY]->Set_ColIndex(COL_REALBODY);
    m_pColliders[HITBOX]->Set_ColIndex(COL_PLAYERHITBOX);
    m_pColliders[RIGHTHAND]->Set_ColIndex(COL_PLAYERATTACK);
    m_pColliders[RIGHTFOOT]->Set_ColIndex(COL_PLAYERATTACK);
     m_pColliders[LEFTHAND]->Set_ColIndex(COL_PLAYERATTACK);
     m_pColliders[LEFTFOOT]->Set_ColIndex(COL_PLAYERATTACK);
     m_pColliders[NINZASWORD]->Set_ColIndex(COL_PLAYERATTACK);
     m_pColliders[SKILLRANGE]->Set_ColIndex(COL_SKILLRANGE);

    return S_OK;
}

HRESULT CUnit_Player::Start()
{
    m_eStartPose = ANIM_WIN_TYPE_3;
    __super::Start();

    return S_OK;
}

void CUnit_Player::My_Tick()
{
   /* if (fDT < 0.016f)
    {
        int i = 0;
    }*/

    if (KEY(CTRL, TAP))
    {
        if (m_pTargetUnit)
        {
            On_FixedTarget(!m_bFixedTarget);

            if (m_bFixedTarget)
                GAMEINSTANCE->Play_Sound(L"LockOn", (CHANNEL_GROUP)CHANNEL_UI);
            else
                GAMEINSTANCE->Play_Sound(L"LockOff", (CHANNEL_GROUP)CHANNEL_UI);

        }
    }

    if (m_eStyleType != STYLE_NIGHTGUY)
    {

        if (KEY(NUM1, TAP))
        {
            m_eStyleType = STYLE_HAND;
            CUser::Get_Instance()->Set_SkillCoolTime(STYLE_HAND, 1, 6.f);
            CUser::Get_Instance()->On_StyleChange(m_eStyleType);
        }
        else if (KEY(NUM2, TAP))
        {
            m_eStyleType = STYLE_NINZASWORD;
            CUser::Get_Instance()->Set_SkillCoolTime(STYLE_NINZASWORD, 1, 1.f);
            CUser::Get_Instance()->On_StyleChange(m_eStyleType);

        }
        else if (KEY(NUM3, TAP))
        {
            m_eStyleType = STYLE_WATER;
            CUser::Get_Instance()->Set_SkillCoolTime(STYLE_WATER, 1, 7.f);
            CUser::Get_Instance()->On_StyleChange(m_eStyleType);

        }
    }



    //타겟 찾기
    //공격중이 아닐 때만 && 타겟 고정 아닐 때만
    if (!m_bFixedTarget)
    {
        if  (m_eCurState < STATE_HANDATTACK_1_PLAYER)
        {
            m_pTargetUnit = CUser::Get_Instance()->Find_ClosestEnemy();
            CUser::Get_Instance()->Set_Target(m_pTargetUnit);
        }
       
    }

    //상태 패턴
    __super::My_Tick();




    _float4 vWorldPos = m_pTransform->Get_World(WORLD_POS);

    _float4x4 matView = GAMEINSTANCE->Get_CurViewMatrix();
    _float4x4 matProj = GAMEINSTANCE->Get_CurProjMatrix();
    _float4 vViewPos = vWorldPos.MultiplyCoord(matView);



    _float4 vTemp = XMVector4Transform(vViewPos.XMLoad(), matProj.XMLoad());


    _float4 vProjPos = vViewPos.MultiplyCoord(matProj);


}

void CUnit_Player::OnEnable()
{
    __super::OnEnable();
    CState_Manager::Get_Instance()->Get_State(STATE_POSE)->Set_AnimType(m_eStartPose);
    Enter_State(m_eCurState);

    for (auto& elem : m_TrailEffects)
    ENABLE_GAMEOBJECT(elem);

   /* for (_uint i = RIGHTHAND; i < PARTS_END; ++i)
        DISABLE_COMPONENT(m_pColliders[i]);*/
}

void CUnit_Player::OnDisable()
{
    //__super::OnDisable();
    for (auto& elem : m_TrailEffects)
        DISABLE_GAMEOBJECT(elem);

}

