#include "stdafx.h"
#include "CLevel_MainMenu.h"

#include "GameInstance.h"
#include "CGameObject_Factory.h"
#include "CComponent_Factory.h"

#include "CUser.h"

#include "CDefault_UI.h"
#include "CScript_Logo.h"

#include "CDarkScreen.h"
#include "Transform.h"

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
    CGameObject* pDarkScreen = CGameObject_Factory::Clone_GameObject<CDarkScreen>();

    _float4 vPos = pDarkScreen->Get_Transform()->Get_MyWorld(WORLD_POS);
    vPos.z = 1.f;
    pDarkScreen->Get_Transform()->Set_World(WORLD_POS, vPos);

    Ready_GameObject(pDarkScreen, GROUP_UI);

    vector<CGameObject*>    vecUI = CGameObject_Factory::Create_vecUI(L"mainmenu");
    for (_uint i = 0; i < vecUI.size(); ++i)
    {
        Ready_GameObject(vecUI[i], GROUP_UI);
    }


    return S_OK;
}


HRESULT CLevel_MainMenu::Enter()
{
    __super::Enter();

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
    if (FAILED(CGameInstance::Get_Instance()->Render_Font(TEXT("Font_Arial"), L"메인메뉴", _float2(10.f, 10.f), _float4(1.f, 1.f, 1.f, 1.f))))
        return E_FAIL;

    return S_OK;
}

HRESULT CLevel_MainMenu::Exit()
{
    __super::Exit();
    return S_OK;
}

