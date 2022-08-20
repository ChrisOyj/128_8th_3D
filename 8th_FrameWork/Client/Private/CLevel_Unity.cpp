#include "stdafx.h"
#include "CLevel_Unity.h"

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

CLevel_Unity::CLevel_Unity()
{
}

CLevel_Unity::~CLevel_Unity()
{
}

CLevel_Unity* CLevel_Unity::Create()
{
    if (FAILED(CImGui_Manager::Get_Instance()->Initialize()))
        return nullptr;

    CLevel_Unity* pLevel = new CLevel_Unity();

    pLevel->Initialize();

    return pLevel;
}

HRESULT CLevel_Unity::Initialize()
{
    CGameInstance::Get_Instance()->Add_GameObject_Prototype(CTestObj::Create());
    CGameInstance::Get_Instance()->Add_GameObject_Prototype(CTerrain::Create());

    return S_OK;
}

HRESULT CLevel_Unity::SetUp_Prototypes()
{
    //Ready_GameObject(CGameObject_Factory::Clone_GameObject<CTestObj>(), GROUP_PROP);
    Ready_GameObject(CGameObject_Factory::Clone_GameObject<CSkyBox>(), GROUP_DEFAULT);
    //Ready_GameObject(CGameObject_Factory::Clone_GameObject<CTerrain>(), GROUP_DEFAULT);

    Ready_GameObject(CObject_Map::Create("../bin/resources/meshes/environments/mapassets/KonohaForest"), GROUP_DEFAULT);

    m_fLoadingFinish = 1.f;

    return S_OK;
}

HRESULT CLevel_Unity::Enter()
{
    __super::Enter();

    CGameInstance::Get_Instance()->Change_Camera(L"Free");

    return S_OK;
}

void CLevel_Unity::Tick()
{
    CImGui_Manager::Get_Instance()->Tick();

    CUser::Get_Instance()->KeyInput_FPSSetter();


   /* _float4 vPos = m_pTestObj->Get_Transform()->Get_World(WORLD_POS);
    
    if (KEY(U, HOLD))
        vPos.z += 50.f * fDT;
    else if (KEY(J, HOLD))
        vPos.z -= 50.f * fDT;
    else if (KEY(I, HOLD))
        vPos.y -= 50.f * fDT;
    else if (KEY(Y, HOLD))
        vPos.y += 50.f * fDT;

    m_pTestObj->Get_Transform()->Set_World(WORLD_POS, vPos);*/
}

void CLevel_Unity::Late_Tick()
{
}

HRESULT CLevel_Unity::Render()
{
    if (FAILED(CGameInstance::Get_Instance()->Render_Font(TEXT("Font_Arial"), L"유니티 레벨", _float2(10.f, 10.f), _float4(1.f, 1.f, 1.f, 1.f))))
        return E_FAIL;

    if (FAILED(CImGui_Manager::Get_Instance()->Render()))
        return E_FAIL;

    return S_OK;
}

HRESULT CLevel_Unity::Exit()
{
    __super::Exit();

    return S_OK;
}


