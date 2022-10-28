#include "stdafx.h"
#include "CLevel_Stage1.h"

#include "GameInstance.h"
#include "CGameObject_Factory.h"

#include "CUser.h"

#include "ImGui_Manager.h"
#include "Loading_Manager.h"

#include "CScreenEffect.h"
#include "Transform.h"
#include "CTestObj.h"
#include "CSkyBox.h"
#include "CTerrain.h"
#include "CMap.h"
#include "CInstancingMap.h"
#include "CObject_Map.h"
#include "CUnit.h"
#include "CUnit_Player.h"

#include "CGround.h"

#include "CCamera_Follow.h"
#include "CScript_FollowCam.h"
#include "CState_Manager.h"
#include "CWire_Player.h"
#include "CDefault_UI.h"
#include "CCamera_Cinema.h"

#include "CCrossHair.h"

#include "CUnit_Enemy.h"
#include "CEffects_Factory.h"


#include "Renderer.h"
CLevel_Stage1::CLevel_Stage1()
{
}

CLevel_Stage1::~CLevel_Stage1()
{
}

CLevel_Stage1* CLevel_Stage1::Create()
{
    CLevel_Stage1* pLevel = new CLevel_Stage1();

    pLevel->Initialize();

    return pLevel;
}

HRESULT CLevel_Stage1::Initialize()
{

    return S_OK;
}

HRESULT CLevel_Stage1::SetUp_Prototypes()
{
    if (FAILED(CEffects_Factory::Get_Instance()->Initialize()))
        return E_FAIL;

    //Ready_GameObject(CGameObject_Factory::Clone_GameObject<CTestObj>(), GROUP_PROP);
    Ready_GameObject(CGameObject_Factory::Clone_GameObject<CSkyBox>(), GROUP_DEFAULT);
    //Ready_GameObject(CGameObject_Factory::Clone_GameObject<CTerrain>(), GROUP_DEFAULT);
    m_fLoadingFinish = 0.1f;

    m_fLoadingFinish = 0.25f;

    CObject_Map* pMap = CObject_Map::Create(L"ExamArena");
    Ready_GameObject(pMap, GROUP_DEFAULT);
    m_vecStaticShadowObjects = pMap->Get_vecGameObjects();

    m_fLoadingFinish = 0.5f;

    CInstancingMap* pInstanceMap = CInstancingMap::Create(L"../bin/resources/meshes/Environments/foliage/SM_ENV_FOLIAGE_Bush01.fbx", L"ExamArenaBush");
    Ready_GameObject(pInstanceMap, GROUP_DECORATION);
    m_vecStaticShadowObjects.push_back(pInstanceMap);

    pInstanceMap = CInstancingMap::Create(L"../bin/resources/meshes/Environments/foliage/SM_ENV_FOLIAGE_Grass03.fbx", L"ExamArenaGrass");
    Ready_GameObject(pInstanceMap, GROUP_DECORATION);
    m_vecStaticShadowObjects.push_back(pInstanceMap);

    pInstanceMap = CInstancingMap::Create(L"../bin/resources/meshes/Environments/foliage/SM_ENV_FOLIAGE_Flower_Purple01.fbx", L"ExamArenaFlower_Purple");
    Ready_GameObject(pInstanceMap, GROUP_DECORATION);
    m_vecStaticShadowObjects.push_back(pInstanceMap);

    //Instancing Test
    /*

    Ready_GameObject(
        CInstancingMap::Create(L"../bin/resources/meshes/Environments/foliage/SM_ENV_FOLIAGE_Flower_White01.fbx", L"ExamArenaFlower_White"),
        GROUP_DECORATION);

    Ready_GameObject(
        CInstancingMap::Create(L"../bin/resources/meshes/Environments/foliage/SM_ENV_FOLIAGE_Flower_Yellow02.fbx", L"ExamArenaFlower_Yellow"),
        GROUP_DECORATION);*/

    m_fLoadingFinish = 0.6f;


    CTerrain* pTerrain = CTerrain::Create(L"ExamArena");
    Ready_GameObject(pTerrain, GROUP_DEFAULT);
    m_fLoadingFinish = 0.7f;

    CUnit_Player* pPlayer = CUnit_Player::Create(CUser::Get_Instance()->Get_PlayerModelData(), pTerrain->Get_Cell(31396));
   
    CREATE_STATIC(pPlayer, HASHCODE(CUnit_Player));
    DISABLE_GAMEOBJECT(pPlayer);

    /*Light Test*/
   /* LIGHTDESC			LightDesc;
    ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

    LightDesc.eType = tagLightDesc::TYPE_POINT;
    LightDesc.fRange = 10.f;
    LightDesc.vPosition = pPlayer->Get_Transform()->Get_MyWorld(WORLD_POS);
    LightDesc.vPosition.y += 5.f;
    LightDesc.vDiffuse = _float4(0.1f, 0.6f, 1.f, 1.f);
    LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
    LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

    if (FAILED(GAMEINSTANCE->Add_Light(LightDesc)))
        return E_FAIL;*/

    /* Free Camera */
    CCamera_Follow* pFollowCam = CCamera_Follow::Create(pPlayer, pTerrain->Get_Cell(31396));
    pFollowCam->Initialize();
    CREATE_STATIC(pFollowCam, HASHCODE(CCamera_Follow));
    DISABLE_GAMEOBJECT(pFollowCam);
    CGameInstance::Get_Instance()->Add_Camera(L"PlayerCam", pFollowCam);

    /* Skill Cam */
    CCamera_Cinema* pCamCinema = CCamera_Cinema::Create();
    //일부러 이니셜라이즈안함
    CREATE_STATIC(pCamCinema, Convert_ToHash(L"SkillCam"));
    GAMEINSTANCE->Add_Camera(L"Skill", pCamCinema);
    DISABLE_GAMEOBJECT(pCamCinema);
    pCamCinema->Set_Parent(pPlayer);
    pCamCinema->On_LoadCinema("KAMUI", CCamera_Cinema::CINEMA_KAMUI);
    pCamCinema->On_LoadCinema("EIGHTGATES", CCamera_Cinema::CINEMA_EIGHTGATES);
    pCamCinema->On_LoadCinema("NIGHTGUY", CCamera_Cinema::CINEMA_NIGHTGUY);
    pCamCinema->On_LoadCinema("KOTO", CCamera_Cinema::CINEMA_KOTO);
    pCamCinema->On_LoadCinema("ENDING", CCamera_Cinema::CINEMA_ENDING);
    pCamCinema->On_LoadCinema("WATER", CCamera_Cinema::CINEMA_WATER);
    CUser::Get_Instance()->Set_SkillCinemaCam(pCamCinema);
    //pCamCinema->On_LoadCinema("RasenSuriken", CCamera_Cinema::CINEMA_RASENSURIKEN);
    //pCamCinema->On_LoadCinema("Kamui", CCamera_Cinema::CINEMA_KAMUI);

    m_fLoadingFinish = 0.8f;


#ifdef _DEBUG
    CImGui_Manager::Get_Instance()->SetUp_TestWindow(pPlayer, GET_COMPONENT_FROM(pFollowCam, CScript_FollowCam));
    CImGui_Manager::Get_Instance()->Set_SkillCam(pCamCinema);
#endif

    CUser::Get_Instance()->Set_FollowCam(GET_COMPONENT_FROM(pFollowCam, CScript_FollowCam));
    CUser::Get_Instance()->Set_Player(pPlayer);

    static_cast<CWire_Player*>(CState_Manager::Get_Instance()->Get_State(STATE_WIRE_PLAYER))
        ->Set_Terrain(pTerrain);

    UNIT_MODEL_DATA tSandBack;

    tSandBack.bOverall = true;
    tSandBack.iEyeIndex = 0;
    tSandBack.strModelPaths[MODEL_PART_FACE] = L"../bin/resources/meshes/characters/custom/face/SK_CHR_FaceMask.fbx";
    tSandBack.strModelPaths[MODEL_PART_HEADGEAR] = L"../bin/resources/meshes/characters/custom/head/SK_CHR_Man_ShortSpikyHair_09.fbx";
    tSandBack.strModelPaths[MODEL_PART_OVERALL] = L"../bin/resources/meshes/characters/custom/overall/SK_CHR_Man_DarkSRK_01.fbx";

    

    tSandBack.eStartPose = ANIM_WIN_TYPE_2;

    CUnit_Enemy* pEnemy = CUnit_Enemy::Create(tSandBack, pTerrain->Get_Cell(110936));
    Ready_GameObject(pEnemy, GROUP_ENEMY);
    pEnemy->Set_SkillType(0);


    tSandBack.eStartPose = ANIM_WIN_TYPE_DANCE;
    tSandBack.iEyeIndex = 1;
    tSandBack.strModelPaths[MODEL_PART_HEADGEAR] = L"../bin/resources/meshes/characters/custom/head/SK_CHR_Man_HatTurban_01.fbx";
    tSandBack.strModelPaths[MODEL_PART_FACE] = L"../bin/resources/meshes/characters/custom/face/SK_CHR_FaceBandage2.fbx";
    tSandBack.strModelPaths[MODEL_PART_OVERALL] = L"../bin/resources/meshes/characters/custom/overall/SK_CHR_Man_Beach_01_B.fbx";

    pEnemy = CUnit_Enemy::Create(tSandBack, pTerrain->Get_Cell(110926));
    Ready_GameObject(pEnemy, GROUP_ENEMY);
    pEnemy->Set_SkillType(1);



    tSandBack.eStartPose = ANIM_WIN_TYPE_BOX;
    tSandBack.iEyeIndex = 2;
    tSandBack.strModelPaths[MODEL_PART_HEADGEAR] = L"../bin/resources/meshes/characters/custom/head/SK_CHR_Man_Hood_01.fbx";
    tSandBack.strModelPaths[MODEL_PART_LWEAPON] = L"../bin/resources/meshes/weapons/SM_WEP_BoxingGloves_L.fbx";
    tSandBack.strModelPaths[MODEL_PART_RWEAPON] = L"../bin/resources/meshes/weapons/SM_WEP_BoxingGloves_R.fbx";
    tSandBack.strModelPaths[MODEL_PART_OVERALL] = L"../bin/resources/meshes/characters/custom/overall/SK_CHR_Man_DarkSRK_01.fbx";

    tSandBack.strRefBoneName[MODEL_PART_LWEAPON] = "LeftHand";
    tSandBack.strRefBoneName[MODEL_PART_RWEAPON] = "RightHand";

    pEnemy = CUnit_Enemy::Create(tSandBack, pTerrain->Get_Cell(110318));
    Ready_GameObject(pEnemy, GROUP_ENEMY);
    pEnemy->Set_SkillType(2);


    CEffects_Factory::Get_Instance()->On_EnterLevel();


    LIGHTDESC			LightDesc;

    LightDesc.eType = tagLightDesc::TYPE_POINT;
    LightDesc.vPosition = _float4(200.f, 400.f, -200.f, 1.f);
    LightDesc.fRange = 1500.f;
    LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
    LightDesc.vAmbient = _float4(0.3f, 0.3f, 0.3f, 1.f);
    LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

    if (FAILED(GAMEINSTANCE->Add_Light(LightDesc)))
        return E_FAIL;


    m_fLoadingFinish = 1.f;





    return S_OK;
}

HRESULT CLevel_Stage1::Enter()
{
    __super::Enter();

    GAMEINSTANCE->Set_ChannelVolume((CHANNEL_GROUP)CHANNEL_ENVIRONMENT, 0.3f);
    GAMEINSTANCE->Set_ChannelVolume((CHANNEL_GROUP)CHANNEL_EFFECTS, 0.3f);
    

    

    CGameInstance::Get_Instance()->Change_Camera(L"Cinema");
    ENABLE_GAMEOBJECT(PLAYER);

    CUser::Get_Instance()->On_EnableIngameUIs();
    CUser::Get_Instance()->Disable_StaticUIs();

    CUser::Get_Instance()->Start_Cinema(0);
    CUser::Get_Instance()->On_CinemaIntroTurnOn(0);

    if (FAILED(GAMEINSTANCE->Check_Group(COL_BODY, COL_BODY)))
        return E_FAIL;

    if (FAILED(GAMEINSTANCE->Check_Group(COL_WALL, COL_REALBODY)))
        return E_FAIL;

    if (FAILED(GAMEINSTANCE->Check_Group(COL_WALL, COL_PLAYERSKILL)))
        return E_FAIL;

    if (FAILED(GAMEINSTANCE->Check_Group(COL_WALL, COL_ENEMYSKILL)))
        return E_FAIL;

    if (FAILED(GAMEINSTANCE->Check_Group(COL_WALL, COL_PLAYERTHROW)))
        return E_FAIL;

    if (FAILED(GAMEINSTANCE->Check_Group(COL_WALL, COL_ENEMYTHROW)))
        return E_FAIL;

    if (FAILED(GAMEINSTANCE->Check_Group(COL_PLAYERSPECIAL, COL_ENEMYHITBOX)))
        return E_FAIL;

    if (FAILED(GAMEINSTANCE->Check_Group(COL_PLAYERSPECIAL, COL_WALL)))
        return E_FAIL;

    if (FAILED(GAMEINSTANCE->Check_Group(COL_PLAYERATTACK, COL_ENEMYHITBOX)))
        return E_FAIL;

    if (FAILED(GAMEINSTANCE->Check_Group(COL_PLAYERTHROW, COL_ENEMYHITBOX)))
        return E_FAIL;

    if (FAILED(GAMEINSTANCE->Check_Group(COL_ENEMYATTACK, COL_PLAYERHITBOX)))
        return E_FAIL;

    if (FAILED(GAMEINSTANCE->Check_Group(COL_ENEMYTHROW, COL_PLAYERHITBOX)))
        return E_FAIL;

    if (FAILED(GAMEINSTANCE->Check_Group(COL_SKILLRANGE, COL_BODY)))
        return E_FAIL;

    if (FAILED(GAMEINSTANCE->Check_Group(COL_PLAYERSKILL, COL_ENEMYHITBOX)))
        return E_FAIL;

    if (FAILED(GAMEINSTANCE->Check_Group(COL_ENEMYSKILL, COL_PLAYERHITBOX)))
        return E_FAIL;

    //CUser::Get_Instance()->Set_CrossHair(0);
    return S_OK;
}

void CLevel_Stage1::Tick()
{
    if (!m_bStaticShadow)
    {
        m_fShadowDelay += fDT;
        if (m_fShadowDelay > 0.2f)
        {
            for (auto iter = m_vecStaticShadowObjects.begin(); iter != m_vecStaticShadowObjects.end();)
            {
                if (dynamic_cast<CGround*>(*iter))
                    iter = m_vecStaticShadowObjects.erase(iter);
                else
                    ++iter;
            }

            GAMEINSTANCE->Bake_StaticShadow(m_vecStaticShadowObjects, 250.f);
            m_bStaticShadow = true;
        }
    }

    if (KEY(ESC, TAP))
    {
        CLoading_Manager::Get_Instance()->Reserve_Load_Level(LEVEL_STAGE2);
    }
#ifdef _DEBUG
    CImGui_Manager::Get_Instance()->Tick();
#endif
    CUser::Get_Instance()->KeyInput_FPSSetter();
    CUser::Get_Instance()->Fix_CursorPosToCenter();
    CUser::Get_Instance()->Update_KeyCommands();
    CUser::Get_Instance()->Update_WireMode();

}

void CLevel_Stage1::Late_Tick()
{
    CUser::Get_Instance()->SetUp_NoiseResources();

}

HRESULT CLevel_Stage1::Render()
{
  /*  if (FAILED(CGameInstance::Get_Instance()->Render_Font(TEXT("Font_Arial"), L"스테이지1", _float2(10.f, 10.f), _float4(1.f, 1.f, 1.f, 1.f))))
        return E_FAIL;*/

#ifdef _DEBUG

    if (FAILED(CImGui_Manager::Get_Instance()->Render()))
        return E_FAIL;
#endif
    return S_OK;
}

HRESULT CLevel_Stage1::Exit()
{
    CUser::Get_Instance()->Disable_StaticUIs();
    CEffects_Factory::Get_Instance()->On_ExitLevel();
    DISABLE_GAMEOBJECT(PLAYER);

    __super::Exit();

    return S_OK;
}


