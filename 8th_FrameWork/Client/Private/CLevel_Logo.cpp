#include "stdafx.h"
#include "CLevel_Logo.h"

#include "GameInstance.h"
#include "CGameObject_Factory.h"
#include "CComponent_Factory.h"
#include "CTestObj.h"

#include "CUser.h"

#include "CScript_Logo.h"

CLevel_Logo::CLevel_Logo()
{
}

CLevel_Logo::~CLevel_Logo()
{
}

CLevel_Logo* CLevel_Logo::Create()
{
    CLevel_Logo* pLevel = new CLevel_Logo();

    return pLevel;
}

HRESULT CLevel_Logo::Initialize()
{
    CScript_Logo* pScript = CScript_Logo::Create(CP_AFTER_TRANSFORM, LEVEL_MAINMENU);
    CGameInstance::Get_Instance()->Add_Component_Prototype(131111, pScript);

    return S_OK;
}

HRESULT CLevel_Logo::Enter()
{
    CGameObject* pLogoUI = CGameObject_Factory::Create_FromJson(100);
    pLogoUI->Add_Component(CComponent_Factory::Create_FromPrototype(131111, pLogoUI));
    CREATE_GAMEOBJECT(pLogoUI, GROUP_UI);

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
    if (FAILED(CGameInstance::Get_Instance()->Render_Font(TEXT("Font_Arial"), L"로고 화면", _float2(10.f, 10.f), _float4(1.f, 1.f, 1.f, 1.f))))
        return E_FAIL;

    return S_OK;
}

HRESULT CLevel_Logo::Exit()
{
    return S_OK;
}
