#include "stdafx.h"
#include "CObject_Map.h"

#include "GameInstance.h"

#include "Texture.h"
#include "CShader.h"
#include "CModel_Renderer.h"
#include "Transform.h"
#include "Functor.h"
#include "CFader.h"
#include "CMesh_Rect.h"
#include "CMesh_Terrain.h"
#include "Model.h"
#include "CMap.h"
#include "CGround.h"

#include "CComponent_Factory.h"
#include "CPrototype_Factory.h"

#include "CUtility_Json.h"

#include "CWindow_Map.h"





CObject_Map::CObject_Map()
{
}

CObject_Map::~CObject_Map()
{
}

CObject_Map* CObject_Map::Create(const _tchar* pJsonFileName)
{
    CObject_Map* pInstance = new CObject_Map;

    if (FAILED(pInstance->SetUp_ObjectsMap(pJsonFileName)))
    {
        Call_MsgBox(L"Failed to SetUp_ObjectsMap : CObject_Map");
        SAFE_DELETE(pInstance);
        return nullptr;
    }

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Call_MsgBox(L"Failed to Initialize_Prototype : CObject_Map");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CObject_Map::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CObject_Map::Initialize()
{
    return S_OK;
}

HRESULT CObject_Map::Start()
{
    __super::Start();

    for (auto& elem : m_vecGameObjects)
        CREATE_GAMEOBJECT(elem, GROUP_DECORATION);

    return S_OK;
}

HRESULT CObject_Map::SetUp_ObjectsMap(const _tchar* pJsonFileName)
{
    json MapJson;
    if (FAILED(CUtility_Json::Load_Json(CUtility_Json::Complete_Path(pJsonFileName).c_str(), & MapJson)))
        return E_FAIL;

    for (auto iter = MapJson.begin(); iter != MapJson.end(); iter++)
    {
        CGameObject* pGameObject = nullptr;

        json ObjJson = *iter;

        TOOL_MAP_TYPE eType = ObjJson["eClassType"];
        wstring wstrFilePath = ObjJson["wstrFilePath"];
        
        switch (eType)
        {
        case Client::TOOL_MAP_PROP:
            pGameObject = CMap::Create(wstrFilePath);

            break;
        case Client::TOOL_MAP_DEFAULT:
            pGameObject = CMap::Create(wstrFilePath);
            break;
        case Client::TOOL_MAP_GROUND:
            pGameObject = CGround::Create(wstrFilePath);
            break;

        case Client::TOOL_MAP_END:
            break;

        default:
            break;
        }

        if (!pGameObject)
            return E_FAIL;


        if (FAILED(pGameObject->Initialize()))
        {
            return E_FAIL;
        }

        pGameObject->Get_Transform()->Get_Transform().matMyWorld = CUtility_Json::Get_MatrixFromJson(ObjJson["worldMatrix"]);

        m_vecGameObjects.push_back(pGameObject);
    }

    return S_OK;
}
