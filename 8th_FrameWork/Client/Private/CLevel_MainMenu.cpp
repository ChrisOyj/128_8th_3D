#include "stdafx.h"
#include "CLevel_MainMenu.h"

#include "GameInstance.h"
#include "CGameObject_Factory.h"
#include "CComponent_Factory.h"

#include "CUser.h"

#include "CDefault_UI.h"
#include "CScript_Logo.h"
#include "CFader.h"
#include "CDarkScreen.h"
#include "Transform.h"
#include "CScript_MainStart.h"

CLevel_MainMenu::CLevel_MainMenu()
{
}

CLevel_MainMenu::~CLevel_MainMenu()
{
}

HRESULT CLevel_MainMenu::Initialize()
{
    if (FAILED(SetUp_Prototypes()))
        return E_FAIL;

    return S_OK;
}

HRESULT CLevel_MainMenu::SetUp_Prototypes()
{
    vector<CGameObject*>    vecUI = CGameObject_Factory::Create_vecUI(L"MAINMENU");
    for (_uint i = 0; i < vecUI.size(); ++i)
    {
        GET_COMPONENT_FROM(vecUI[i], CFader)->Force_KeyInput();
        Ready_GameObject(vecUI[i], GROUP_UI);
    }

    CDefault_UI* pStartButton = CGameObject_Factory::Create_UI(L"MAINMENU2_StartButton");
    pStartButton->Add_Component(CScript_MainStart::Create(CP_AFTER_TRANSFORM, vecUI));
    Ready_GameObject(pStartButton, GROUP_UI);
    GET_COMPONENT_FROM(pStartButton, CFader)->Force_KeyInput();




    return S_OK;
}


HRESULT CLevel_MainMenu::Enter()
{
    __super::Enter();
    GAMEINSTANCE->Play_BGM(L"BGM_MainMenu");
    GAMEINSTANCE->Play_Sound(L"MainMenuStart", (CHANNEL_GROUP)CHANNEL_UI);

    return S_OK;
}

void CLevel_MainMenu::Tick()
{
}

void CLevel_MainMenu::Late_Tick()
{
}

HRESULT CLevel_MainMenu::Render()
{

    return S_OK;
}

HRESULT CLevel_MainMenu::Exit()
{
    //CGameInstance::Get_Instance()->Delete_Objects(GROUP_UI);

    __super::Exit();
    return S_OK;
}

