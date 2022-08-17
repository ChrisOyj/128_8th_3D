#include "stdafx.h"
#include "CLevel_Logo.h"

#include "GameInstance.h"
#include "CGameObject_Factory.h"
#include "CComponent_Factory.h"

#include "CUser.h"

#include "CDefault_UI.h"
#include "CScript_Logo.h"

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

    Ready_GameObject(pLogo, GROUP_UI);
    Ready_GameObject(pLogoBG, GROUP_UI);


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
    if (FAILED(CGameInstance::Get_Instance()->Render_Font(TEXT("Font_Arial"), L"로고 화면", _float2(10.f, 10.f), _float4(1.f, 1.f, 1.f, 1.f))))
        return E_FAIL;

    return S_OK;
}

HRESULT CLevel_Logo::Exit()
{
    __super::Exit();
    return S_OK;
}

