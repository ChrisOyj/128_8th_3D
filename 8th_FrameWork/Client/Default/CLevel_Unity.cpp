#include "stdafx.h"
#include "CLevel_Unity.h"

#include "GameInstance.h"
#include "CGameObject_Factory.h"
#include "CTestObj.h"

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

HRESULT CLevel_Unity::Enter()
{
    CGameInstance::Get_Instance()->Add_GameObject_Prototype(200000, CTestObj::Create());
    
    CREATE_GAMEOBJECT(CGameObject_Factory::Create_FromPrototype(200000), GROUP_PROP);



    return S_OK;
}

void CLevel_Unity::Tick()
{
}

void CLevel_Unity::Late_Tick()
{
}

HRESULT CLevel_Unity::Render()
{
    return S_OK;
}

HRESULT CLevel_Unity::Exit()
{
    return S_OK;
}
