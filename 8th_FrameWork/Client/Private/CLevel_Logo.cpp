#include "stdafx.h"
#include "CLevel_Logo.h"

#include "GameInstance.h"
#include "CGameObject_Factory.h"
#include "CComponent_Factory.h"

#include "CUser.h"

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

    return S_OK;
}

HRESULT CLevel_Logo::Enter()
{

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

HRESULT CLevel_Logo::SetUp_Prototypes()
{
    return E_NOTIMPL;
}
