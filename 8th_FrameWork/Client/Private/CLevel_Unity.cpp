#include "stdafx.h"
#include "CLevel_Unity.h"

#include "GameInstance.h"
#include "CGameObject_Factory.h"
#include "CTestObj.h"

#include "CUser.h"

CLevel_Unity::CLevel_Unity()
{
}

CLevel_Unity::~CLevel_Unity()
{
}

CLevel_Unity* CLevel_Unity::Create()
{
    CLevel_Unity* pLevel = new CLevel_Unity();

    return pLevel;
}

HRESULT CLevel_Unity::Initialize()
{
    if (FAILED(CGameInstance::Get_Instance()->Add_GameObject_Prototype(200000, CTestObj::Create())))
        return E_FAIL;

    //if (FAILED(CGameInstance::Get_Instance()->Add_Font(TEXT("Font_Dream"), TEXT("../Bin/Resources/Fonts/128.spriteFont"))))
       // return E_FAIL;

    for (_uint i = 0; i < 99999999; ++i)
    {
        int a = 0;
    }

    for (_uint i = 0; i < 99999999; ++i)
    {
        int a = 0;
    }

    for (_uint i = 0; i < 99999999; ++i)
    {
        int a = 0;
    }

    for (_uint i = 0; i < 99999999; ++i)
    {
        int a = 0;
    }

    return S_OK;
}

HRESULT CLevel_Unity::Enter()
{
    CREATE_GAMEOBJECT(CGameObject_Factory::Create_FromPrototype(200000), GROUP_PROP);
    CGameInstance::Get_Instance()->Change_Camera(L"Free");



    return S_OK;
}

void CLevel_Unity::Tick()
{
    //CUser::Get_Instance()->Fix_CursorPosToCenter();
    CUser::Get_Instance()->KeyInput_FPSSetter();
}

void CLevel_Unity::Late_Tick()
{
}

HRESULT CLevel_Unity::Render()
{
    if (FAILED(CGameInstance::Get_Instance()->Render_Font(TEXT("Font_Arial"), L"폰트 테스트", _float2(10.f, 10.f), _float4(1.f, 1.f, 1.f, 1.f))))
        return E_FAIL;
    return S_OK;
}

HRESULT CLevel_Unity::Exit()
{
    return S_OK;
}
