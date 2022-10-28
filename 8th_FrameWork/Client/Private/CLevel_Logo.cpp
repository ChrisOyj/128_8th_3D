#include "stdafx.h"
#include "CLevel_Logo.h"

#include "GameInstance.h"
#include "CGameObject_Factory.h"
#include "CComponent_Factory.h"

#include "CUser.h"

#include "CDefault_UI.h"
#include "CScript_Logo.h"

#include "Loading_Manager.h"

#include "CLevel_Unity.h"
#include "CLevel_MainMenu.h"
#include "CLevel_Customizing.h"

#include "CCamera_Cinema.h"

#include "ImGui_Manager.h"
CLevel_Logo::CLevel_Logo()
{
}

CLevel_Logo::~CLevel_Logo()
{
}

HRESULT CLevel_Logo::Initialize()
{

    return S_OK;
}

HRESULT CLevel_Logo::SetUp_Prototypes()
{
    CDefault_UI* pLogoBG = CGameObject_Factory::Create_UI(L"Logo_BG");
    CDefault_UI* pLogo = CGameObject_Factory::Create_UI(L"Logo");
    
    pLogo->Add_Component(CScript_Logo::Create(CP_AFTER_TRANSFORM, pLogoBG));

    CLoading_Manager::Get_Instance()->Set_Level(CLevel_MainMenu::Create(), LEVEL_MAINMENU);
    m_fLoadingFinish = 0.5f;

    CLoading_Manager::Get_Instance()->Set_Level(CLevel_Customizing::Create(), LEVEL_CUSTOMIZING);
    m_fLoadingFinish = 0.75f;

    if (FAILED(CUser::Get_Instance()->Initialize()))
        return E_FAIL;

    Ready_GameObject(pLogo, GROUP_UI);
    Ready_GameObject(pLogoBG, GROUP_UI);


    //cam
    CCamera_Cinema* pCamCinema = CCamera_Cinema::Create();
    pCamCinema->Initialize();
    CREATE_STATIC(pCamCinema, HASHCODE(CCamera_Cinema));
    GAMEINSTANCE->Add_Camera(L"Cinema", pCamCinema);
    DISABLE_GAMEOBJECT(pCamCinema);

#ifdef _DEBUG
    CImGui_Manager::Get_Instance()->SetUp_CinemaWindow(pCamCinema);
#endif
    pCamCinema->On_LoadCinema("ExamArena", CCamera_Cinema::CINEMA_EXAMARENA);
    pCamCinema->On_LoadCinema("KonohaVillage", CCamera_Cinema::CINEMA_KONOHAVILLAGE);
    pCamCinema->On_LoadCinema("BOSS", CCamera_Cinema::CINEMA_BOSS);
    CUser::Get_Instance()->Set_CinemaCam(pCamCinema);

    m_fLoadingFinish = 1.f;

    return S_OK;
}


HRESULT CLevel_Logo::Enter()
{
    __super::Enter();

    return S_OK;
}

void CLevel_Logo::Tick()
{
}

void CLevel_Logo::Late_Tick()
{
}

HRESULT CLevel_Logo::Render()
{

    return S_OK;
}

HRESULT CLevel_Logo::Exit()
{
    __super::Exit();
    return S_OK;
}

