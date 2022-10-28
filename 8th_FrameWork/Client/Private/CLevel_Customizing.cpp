#include "stdafx.h"
#include "CLevel_Customizing.h"

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

#include "CFader.h"
#include "Transform.h"
#include "Texture.h"

#include "CRoleWindow_Customizing.h"
#include "CCustomizingPreview.h"

CLevel_Customizing::CLevel_Customizing()
{
}

CLevel_Customizing::~CLevel_Customizing()
{
}

HRESULT CLevel_Customizing::Initialize()
{
    if (FAILED(SetUp_Prototypes()))
        return E_FAIL;
    return S_OK;
}

HRESULT CLevel_Customizing::SetUp_Prototypes()
{
    vector<CGameObject*>    vecUI = CGameObject_Factory::Create_vecUI(L"Customizing");
    for (_uint i = 0; i < vecUI.size(); ++i)
    {
        GET_COMPONENT_FROM(vecUI[i], CFader)->Force_KeyInput();
        Ready_GameObject(vecUI[i], GROUP_UI);
    }

    m_fLoadingFinish = 0.4f;
    CCustomizingPreview* pPreview = CCustomizingPreview::Create(L"../bin/resources/meshes/characters/custom/face/SK_CHR_Face.fbx");
    Ready_GameObject(pPreview, GROUP_DEFAULT);

    m_fLoadingFinish = 0.75f;

    Ready_GameObject(CRoleWindow_Customizing::Create(pPreview), GROUP_UI);

    m_fLoadingFinish = 1.f;


    LIGHTDESC			LightDesc;

    LightDesc.eType = tagLightDesc::TYPE_POINT;
    LightDesc.vPosition = _float4(200.f, 400.f, -200.f, 1.f);
    LightDesc.fRange = 1500.f;
    LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
    LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
    LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

    if (FAILED(GAMEINSTANCE->Add_Light(LightDesc)))
        return E_FAIL;

    return S_OK;
}


HRESULT CLevel_Customizing::Enter()
{
    m_vecGameObjects.back().first->Initialize();

    __super::Enter();
    //CGameInstance::Get_Instance()->Change_Camera(L"Free");

    return S_OK;
}

void CLevel_Customizing::Tick()
{
}

void CLevel_Customizing::Late_Tick()
{
}

HRESULT CLevel_Customizing::Render()
{

    return S_OK;
}

HRESULT CLevel_Customizing::Exit()
{
    __super::Exit();
    return S_OK;
}

