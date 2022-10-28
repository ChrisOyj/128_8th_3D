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
#include "CUnit.h"
#include "CUnit_Player.h"

#include "CScript_Freecam.h"
#include "CCamera_Free.h"
#include "CGoal.h"

CLevel_Unity::CLevel_Unity()
{
}

CLevel_Unity::~CLevel_Unity()
{
}

CLevel_Unity* CLevel_Unity::Create()
{
#ifdef _DEBUG
    if (FAILED(CImGui_Manager::Get_Instance()->Initialize()))
        return nullptr;
#endif

    CLevel_Unity* pLevel = new CLevel_Unity();

    pLevel->Initialize();

    return pLevel;
}

HRESULT CLevel_Unity::Initialize()
{
    CGameInstance::Get_Instance()->Add_GameObject_Prototype(CTestObj::Create());

    return S_OK;
}

HRESULT CLevel_Unity::SetUp_Prototypes()
{
    //Ready_GameObject(CGameObject_Factory::Clone_GameObject<CTestObj>(), GROUP_PROP);
    Ready_GameObject(CGameObject_Factory::Clone_GameObject<CSkyBox>(), GROUP_DEFAULT);
    m_fLoadingFinish = 0.25f;

    MODEL_DATA* pModelData = nullptr;

    m_fLoadingFinish = 1.f;
    //Ready_GameObject(CObject_Map::Create("../bin/resources/meshes/environments/mapassets/KonohaForest"), GROUP_DEFAULT);
    LIGHTDESC			LightDesc;

    LightDesc.eType = tagLightDesc::TYPE_POINT;
    LightDesc.vPosition = _float4(200.f, 400.f, -200.f, 1.f);
    LightDesc.fRange = 1000.f;
    LightDesc.vDiffuse = _float4(0.2f, 0.2f, 0.4f, 1.f);
    LightDesc.vAmbient = _float4(0.3f, 0.3f, 0.4f, 1.f);
    LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

    if (FAILED(GAMEINSTANCE->Add_Light(LightDesc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CLevel_Unity::Enter()
{
    __super::Enter();

    CGameInstance::Get_Instance()->Change_Camera(L"Free");
    GET_COMPONENT_FROM(GAMEINSTANCE->Get_CurCam(), CScript_Freecam)->Enter_UnityLevel();
    

    return S_OK;
}

void CLevel_Unity::Tick()
{
#ifdef _DEBUG

    CImGui_Manager::Get_Instance()->Tick();
#endif
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

    if (FAILED(CImGui_Manager::Get_Instance()->Render()))
        return E_FAIL;

    return S_OK;
}

HRESULT CLevel_Unity::Exit()
{
    __super::Exit();

    return S_OK;
}


