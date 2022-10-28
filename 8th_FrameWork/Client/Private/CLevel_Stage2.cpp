#include "stdafx.h"
#include "CLevel_Stage2.h"

#include "GameInstance.h"
#include "CGameObject_Factory.h"

#include "CUser.h"

#include "ImGui_Manager.h"

#include "Transform.h"
#include "CTestObj.h"
#include "CSkyBox.h"
#include "CTerrain.h"
#include "CMap.h"
#include "CObject_Map.h"
#include "CUnit.h"
#include "CUnit_Player.h"
#include "Physics.h"

#include "CCamera_Follow.h"
#include "CScript_FollowCam.h"
#include "CState_Manager.h"
#include "CWire_Player.h"
#include "CDefault_UI.h"
#include "CNavigation.h"
#include "CCrossHair.h"
#include "CUnit_Enemy.h"
#include "CEffects_Factory.h"
#include "CCell.h"
#include "CGoal.h"
#include "Texture.h"

#include "CInstancingMap.h"
#include "CState_Manager.h"
#include "CGoTarget.h"

#include "CScreenEffect.h"
CLevel_Stage2::CLevel_Stage2()
{
}

CLevel_Stage2::~CLevel_Stage2()
{
}

CLevel_Stage2* CLevel_Stage2::Create()
{
    CLevel_Stage2* pLevel = new CLevel_Stage2();

    pLevel->Initialize();

    return pLevel;
}

HRESULT CLevel_Stage2::Initialize()
{

    return S_OK;
}

HRESULT CLevel_Stage2::SetUp_Prototypes()
{
    CSkyBox* pSkyBox = (CGameObject_Factory::Clone_GameObject<CSkyBox>());
    GET_COMPONENT_FROM(pSkyBox, CTexture)->Set_CurTextureIndex(4);
    Ready_GameObject(pSkyBox, GROUP_DEFAULT);

    m_fLoadingFinish = 0.1f;
   
    CObject_Map *pMap = CObject_Map::Create(L"KonohaVillage");
    Ready_GameObject(pMap, GROUP_DEFAULT);
    m_fLoadingFinish = 0.5f;

    m_vecStaticShadowObjects = pMap->Get_vecGameObjects();

    CInstancingMap* pInstanceMap = CInstancingMap::Create(L"../bin/resources/meshes/Environments/foliage/SM_ENV_FOLIAGE_Grass03.fbx", L"KonohaVillage_Grass");
    Ready_GameObject(pInstanceMap, GROUP_DECORATION);
    m_vecStaticShadowObjects.push_back(pInstanceMap);

    pInstanceMap = CInstancingMap::Create(L"../bin/resources/meshes/Environments/mapassets/konohaVillage/props/SM_ENV_Props_Barrel_01.fbx", L"KonohaVillage_Barrel");
    Ready_GameObject(pInstanceMap, GROUP_DECORATION);
    m_vecStaticShadowObjects.push_back(pInstanceMap);

    pInstanceMap = CInstancingMap::Create(L"../bin/resources/meshes/Environments/foliage/SM_ENV_FOLIAGE_Grass02.fbx", L"KonohaVillage_Grass2");
    Ready_GameObject(pInstanceMap, GROUP_DECORATION);
    m_vecStaticShadowObjects.push_back(pInstanceMap);

    pInstanceMap = CInstancingMap::Create(L"../bin/resources/meshes/Environments/foliage/SM_ENV_FOLIAGE_Flower_White01.fbx", L"KonohaVillage_Flower_White");
    Ready_GameObject(pInstanceMap, GROUP_DECORATION);
    m_vecStaticShadowObjects.push_back(pInstanceMap);

    pInstanceMap = CInstancingMap::Create(L"../bin/resources/meshes/Environments/mapassets/konohaVillage/props/SM_ENV_Props_Crate_01.fbx", L"KonohaVillage_Crate");
    Ready_GameObject(pInstanceMap, GROUP_DECORATION);
    m_vecStaticShadowObjects.push_back(pInstanceMap);

    pInstanceMap = CInstancingMap::Create(L"../bin/resources/meshes/Environments/foliage/SM_ENV_KNVLLG_Plant_A.fbx", L"KonohaVillage_Plant_A");
    Ready_GameObject(pInstanceMap, GROUP_DECORATION);
    m_vecStaticShadowObjects.push_back(pInstanceMap);

    pInstanceMap = CInstancingMap::Create(L"../bin/resources/meshes/Environments/foliage/SM_ENV_KNVLLG_Plant_B.fbx", L"KonohaVillage_Plant_B");
    Ready_GameObject(pInstanceMap, GROUP_DECORATION);
    m_vecStaticShadowObjects.push_back(pInstanceMap);

    pInstanceMap = CInstancingMap::Create(L"../bin/resources/meshes/Environments/foliage/SM_ENV_KNVLLG_Plant_D.fbx", L"KonohaVillage_Plant_D");
    Ready_GameObject(pInstanceMap, GROUP_DECORATION);
    m_vecStaticShadowObjects.push_back(pInstanceMap);

    pInstanceMap = CInstancingMap::Create(L"../bin/resources/meshes/Environments/foliage/SM_ENV_KNVLLG_Plant_E.fbx", L"KonohaVillage_Plant_E");
    Ready_GameObject(pInstanceMap, GROUP_DECORATION);
    m_vecStaticShadowObjects.push_back(pInstanceMap);

    pInstanceMap = CInstancingMap::Create(L"../bin/resources/meshes/Environments/foliage/SM_ENV_KNVLLG_Plant_G.fbx", L"KonohaVillage_Plant_G");
    Ready_GameObject(pInstanceMap, GROUP_DECORATION);
    m_vecStaticShadowObjects.push_back(pInstanceMap);

    pInstanceMap = CInstancingMap::Create(L"../bin/resources/meshes/Environments/mapassets/konohaVillage/objects/SM_ENV_KNVLLG_VideoScreen_02.fbx", L"KonohaVillage_VideoScreen");
    Ready_GameObject(pInstanceMap, GROUP_DECORATION);
    m_vecStaticShadowObjects.push_back(pInstanceMap);

    pInstanceMap = CInstancingMap::Create(L"../bin/resources/meshes/Environments/mapassets/konohaVillage/objects/SM_ENV_KNVLLG_GateUnit_01.fbx", L"KonohaVillage_GateUnit");
    Ready_GameObject(pInstanceMap, GROUP_DECORATION);
    m_vecStaticShadowObjects.push_back(pInstanceMap);

    pInstanceMap = CInstancingMap::Create(L"../bin/resources/meshes/Environments/mapassets/konohaVillage/props/SM_ENV_Manhole.fbx", L"KonohaVillage_Manhole");
    Ready_GameObject(pInstanceMap, GROUP_DECORATION);
    m_vecStaticShadowObjects.push_back(pInstanceMap);
    /*Ready_GameObject(
        CInstancingMap::Create(L"../bin/resources/meshes/Environments/foliage/SM_ENV_FOLIAGE_Flower_Yellow02.fbx", L"KonohaVillage_Flower_Yellow"),
        GROUP_DECORATION);*/

    m_fLoadingFinish = 0.6f;



    m_pTerrain = CTerrain::Create(L"KonohaVillage");
    Ready_GameObject(m_pTerrain, GROUP_DEFAULT);
    m_fLoadingFinish = 0.65f;

    CUser::Get_Instance()->Set_Terrain(m_pTerrain);

    static_cast<CWire_Player*>(CState_Manager::Get_Instance()->Get_State(STATE_WIRE_PLAYER))
        ->Set_Terrain(m_pTerrain);


    UNIT_MODEL_DATA tSandBack;
    tSandBack.eStartPose = ANIM_WIN_TYPE_DANCE2;
    tSandBack.bOverall = true;
    tSandBack.iEyeIndex = 6;
    tSandBack.strModelPaths[MODEL_PART_FACE] = L"../bin/resources/meshes/characters/custom/face/SK_CHR_FaceMask.fbx";
    tSandBack.strModelPaths[MODEL_PART_HEADGEAR] = L"../bin/resources/meshes/characters/custom/head/SK_CHR_Man_PajamaHat_01.fbx";
    tSandBack.strModelPaths[MODEL_PART_OVERALL] = L"../bin/resources/meshes/characters/custom/overall/SK_CHR_Man_Pajamas.fbx";
    tSandBack.strModelPaths[MODEL_PART_ACCESSORY] = L"../bin/resources/meshes/characters/custom/accessories/SK_CHR_Book_MakeOut.fbx";
    

    //BOSS
    CUnit_Enemy* pEnemy = nullptr;
    pEnemy = CUnit_Enemy::Create(tSandBack, m_pTerrain->Get_Cell(47036));
    pEnemy->Disable_AllColliders();
    Ready_GameObject(pEnemy, GROUP_ENEMY);
    pEnemy->Get_Status().fMaxHP = 300.f;
    pEnemy->Get_Status().fHP = 300.f;
    m_fLoadingFinish = 0.7f;
    pEnemy->Set_SkillType2(0);
    pEnemy->Set_SkillType(2);


    //쫄몹 1
    tSandBack.bOverall = false;
    tSandBack.iEyeIndex = 0;
    tSandBack.strModelPaths[MODEL_PART_ACCESSORY] = L"../bin/resources/meshes/characters/custom/accessories/SK_CHR_Glasses_Shooter.fbx";
    tSandBack.strModelPaths[MODEL_PART_FACE] = L"../bin/resources/meshes/characters/custom/face/SK_CHR_FaceMask.fbx";
    tSandBack.strModelPaths[MODEL_PART_HEADGEAR] = L"../bin/resources/meshes/characters/custom/head/SK_CHR_Man_ShortSpikyHair_09.fbx";
    tSandBack.strModelPaths[MODEL_PART_BODYUPPER] = L"../bin/resources/meshes/characters/custom/bodyupper/SK_CHR_Man_AnbuBlackOps_02.fbx";
    tSandBack.strModelPaths[MODEL_PART_BODYLOWER] = L"../bin/resources/meshes/characters/custom/bodylower/SK_CHR_Man_AnbuBlackOps_Lower_02.fbx";
    tSandBack.strModelPaths[MODEL_PART_LWEAPON] = L"../bin/resources/meshes/weapons/SM_WEP_BoxingGloves_L.fbx";
    tSandBack.strModelPaths[MODEL_PART_RWEAPON] = L"../bin/resources/meshes/weapons/SM_WEP_BoxingGloves_R.fbx";
    tSandBack.strRefBoneName[MODEL_PART_LWEAPON] = "LeftHand";
    tSandBack.strRefBoneName[MODEL_PART_RWEAPON] = "RightHand";
    pEnemy = CUnit_Enemy::Create(tSandBack, m_pTerrain->Get_Cell(34596)); // 1번지점 포인트 1
    DISABLE_GAMEOBJECT(pEnemy);
    Ready_GameObject(pEnemy, GROUP_ENEMY);

    //2
    tSandBack.strModelPaths[MODEL_PART_ACCESSORY] = L"../bin/resources/meshes/characters/custom/accessories/SK_CHR_FinalGear.fbx";
    tSandBack.strModelPaths[MODEL_PART_FACE] = L"../bin/resources/meshes/characters/custom/face/SK_CHR_FaceMask2.fbx";
    tSandBack.strModelPaths[MODEL_PART_HEADGEAR] = L"../bin/resources/meshes/characters/custom/head/SK_CHR_Man_TopKnot_01.fbx";
    tSandBack.strModelPaths[MODEL_PART_OVERALL] = L"../bin/resources/meshes/characters/custom/overall/SK_CHR_Man_Overall_05.fbx";
    tSandBack.bOverall = true;

    tSandBack.strModelPaths[MODEL_PART_LWEAPON] .clear();
    tSandBack.strModelPaths[MODEL_PART_RWEAPON] .clear();
    tSandBack.strRefBoneName[MODEL_PART_LWEAPON].clear();
    tSandBack.strRefBoneName[MODEL_PART_RWEAPON].clear();

    pEnemy = CUnit_Enemy::Create(tSandBack, m_pTerrain->Get_Cell(35232)); // 1번지점 포인트 2
    DISABLE_GAMEOBJECT(pEnemy);
    Ready_GameObject(pEnemy, GROUP_ENEMY);
    m_fLoadingFinish = 0.75f;

    //3
    tSandBack.bOverall = false;
    tSandBack.strModelPaths[MODEL_PART_ACCESSORY].clear();
    tSandBack.strModelPaths[MODEL_PART_FACE] = L"../bin/resources/meshes/characters/custom/face/SK_CHR_FaceBandage2.fbx";
    tSandBack.strModelPaths[MODEL_PART_HEADGEAR] = L"../bin/resources/meshes/characters/custom/head/SK_CHR_Man_HatTurban_01.fbx";
    pEnemy = CUnit_Enemy::Create(tSandBack, m_pTerrain->Get_Cell(47194)); // 1번지점 포인트 3
    DISABLE_GAMEOBJECT(pEnemy);
    Ready_GameObject(pEnemy, GROUP_ENEMY);

    //4
    tSandBack.strModelPaths[MODEL_PART_ACCESSORY] = L"../bin/resources/meshes/characters/custom/accessories/SK_LightningOrnaments_02.fbx";
    tSandBack.strModelPaths[MODEL_PART_FACE] = L"../bin/resources/meshes/characters/custom/face/SK_CHR_FaceMask.fbx";
    tSandBack.strModelPaths[MODEL_PART_HEADGEAR] = L"../bin/resources/meshes/characters/custom/head/SK_CHR_Man_Hood_01.fbx";
    pEnemy = CUnit_Enemy::Create(tSandBack, m_pTerrain->Get_Cell(48952)); // 1번지점 포인트 4
    DISABLE_GAMEOBJECT(pEnemy);
    Ready_GameObject(pEnemy, GROUP_ENEMY);

    //5
    tSandBack.strModelPaths[MODEL_PART_FACE] = L"../bin/resources/meshes/characters/custom/face/SK_CHR_FaceBandage2.fbx";
    tSandBack.strModelPaths[MODEL_PART_HEADGEAR] = L"../bin/resources/meshes/characters/custom/head/SK_CHR_Man_Helmet_02.fbx";
    tSandBack.strModelPaths[MODEL_PART_ACCESSORY] = L"../bin/resources/meshes/characters/custom/accessories/SK_WaterOrnaments_01.fbx";

   
    pEnemy = CUnit_Enemy::Create(tSandBack, m_pTerrain->Get_Cell(98542)); // 2번지점 포인트 5
    DISABLE_GAMEOBJECT(pEnemy);
    Ready_GameObject(pEnemy, GROUP_ENEMY);
    m_fLoadingFinish = 0.85f;

    //6
    tSandBack.bOverall = true;
    tSandBack.strModelPaths[MODEL_PART_ACCESSORY] = L"../bin/resources/meshes/characters/custom/accessories/SK_CHR_Bell2.fbx";
    tSandBack.strModelPaths[MODEL_PART_FACE] = L"../bin/resources/meshes/characters/custom/face/SK_CHR_FaceMask2.fbx";
    tSandBack.strModelPaths[MODEL_PART_HEADGEAR] = L"../bin/resources/meshes/characters/custom/head/SK_CHR_Man_ShortSpikyHair_06.fbx";
    tSandBack.strModelPaths[MODEL_PART_OVERALL] = L"../bin/resources/meshes/characters/custom/overall/SK_CHR_Man_Tunakkay.fbx";
    tSandBack.strModelPaths[MODEL_PART_LWEAPON] = L"../bin/resources/meshes/weapons/SM_Gloves_Cat.fbx";
    tSandBack.strModelPaths[MODEL_PART_RWEAPON] = L"../bin/resources/meshes/weapons/SM_Gloves_Cat.fbx";
    tSandBack.strRefBoneName[MODEL_PART_LWEAPON] = "LeftHand";
    tSandBack.strRefBoneName[MODEL_PART_RWEAPON] = "RightHand";
    
    pEnemy = CUnit_Enemy::Create(tSandBack, m_pTerrain->Get_Cell(97988)); // 2번지점 포인트 6
    DISABLE_GAMEOBJECT(pEnemy);
    Ready_GameObject(pEnemy, GROUP_ENEMY);

    //7 Goal
    CGoal* pGoal = CGoal::Create();
    pGoal->Initialize();
    CUser::Get_Instance()->Set_Goal(pGoal);
    Ready_GameObject(pGoal, GROUP_DEFAULT);

    CEffects_Factory::Get_Instance()->On_EnterLevel();

    LIGHTDESC			LightDesc;

    LightDesc.eType = tagLightDesc::TYPE_POINT;
    LightDesc.vPosition = _float4(200.f, 400.f, -200.f, 1.f);
    LightDesc.fRange = 1000.f;
    LightDesc.vDiffuse = _float4(0.4f, 0.4f, 0.6f, 1.f);
    LightDesc.vAmbient = _float4(0.3f, 0.3f, 0.4f, 1.f);
    LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

    if (FAILED(GAMEINSTANCE->Add_Light(LightDesc)))
        return E_FAIL;
    
    if (FAILED(GAMEINSTANCE->Load_Lights(L"KonohaVillage")))
        return E_FAIL;

    

    m_fLoadingFinish = 1.f;
    return S_OK;
}

HRESULT CLevel_Stage2::Enter()
{
    __super::Enter();


    if (FAILED(GAMEINSTANCE->Check_Group(COL_PLAYERHITBOX, COL_EVENT)))
        return E_FAIL;

    CGameInstance::Get_Instance()->Change_Camera(L"Cinema");

    CUser::Get_Instance()->Start_Cinema(1);
    CUser::Get_Instance()->On_CinemaIntroTurnOn(1);
    ENABLE_GAMEOBJECT(PLAYER);


    GET_COMPONENT_FROM(PLAYER, CNavigation)->Set_StartCell(m_pTerrain->Get_StartCell());
    GET_COMPONENT_FROM(GAMEINSTANCE->Find_Camera(L"PlayerCam"), CNavigation)->Set_StartCell(m_pTerrain->Get_StartCell());
    _float4 vStartPos;
    XMStoreFloat4(&vStartPos, m_pTerrain->Get_StartCell()->Get_Point(CCell::POINT_A));
    vStartPos.w = 1.f;
    vStartPos.x += 0.1f;
    vStartPos.z += 0.1f;

    //PLAYER->Enter_State(STATE_IDLE_PLAYER);
    PLAYER->Get_Transform()->Set_World(WORLD_POS, vStartPos);
    PLAYER->Get_Transform()->Set_Look(_float4(0.f, 0.f, 1.f, 0.f));
    PLAYER->Get_Transform()->Make_WorldMatrix();
    CPhysics* pPhysics = GET_COMPONENT_FROM(PLAYER, CPhysics);
    pPhysics->Set_Speed(0.f);
    pPhysics->Get_Physics().bAir = false;
    PLAYER->Enter_State(STATE_POSE);

    GAMEINSTANCE->Find_Camera(L"PlayerCam")->Get_Transform()->Set_World(WORLD_POS, vStartPos);
    GAMEINSTANCE->Find_Camera(L"PlayerCam")->Get_Transform()->Set_Look(_float4(0.f, 0.f, 1.f, 0.f));
    GAMEINSTANCE->Find_Camera(L"PlayerCam")->Get_Transform()->Make_WorldMatrix();
   
    _float4 vGoalPos = m_pTerrain->Get_Cell(41198)->Get_Point(CCell::POINT_A);
    vGoalPos.y += 0.5f;
    vGoalPos.w = 1.f;
    GET_STATE(STATE_GOTARGET, CGoTarget)->Set_TargetPos(vGoalPos);

    CUser::Get_Instance()->Get_Goal()->Get_Transform()->Set_World(WORLD_POS, vGoalPos);
    CUser::Get_Instance()->Get_Goal()->Get_Transform()->Make_WorldMatrix();
    

    return S_OK;
}

void CLevel_Stage2::Tick()
{

    if (!m_bStaticShadow)
    {
        m_fShadowDelay += fDT;
        if (m_fShadowDelay > 0.2f)
        {
            GAMEINSTANCE->Bake_StaticShadow(m_vecStaticShadowObjects, 600.f);
            m_bStaticShadow = true;
        }
    }

#ifdef _DEBUG

    CImGui_Manager::Get_Instance()->Tick();
#endif
    CUser::Get_Instance()->KeyInput_FPSSetter();
    CUser::Get_Instance()->Fix_CursorPosToCenter();
    CUser::Get_Instance()->Update_KeyCommands();
    CUser::Get_Instance()->Update_WireMode();

}

void CLevel_Stage2::Late_Tick()
{
}

HRESULT CLevel_Stage2::Render()
{
#ifdef _DEBUG

    if (FAILED(CImGui_Manager::Get_Instance()->Render()))
        return E_FAIL;
#endif
    return S_OK;
}

HRESULT CLevel_Stage2::Exit()
{
    __super::Exit();

    return S_OK;
}


