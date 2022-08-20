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

#include "CComponent_Factory.h"
#include "CPrototype_Factory.h"





CObject_Map::CObject_Map()
{
}

CObject_Map::~CObject_Map()
{
}

CObject_Map* CObject_Map::Create(const char* pFolderPath)
{
    CObject_Map* pInstance = new CObject_Map;

    if (FAILED(pInstance->SetUp_ObjectsMap(pFolderPath)))
    {
        Call_MsgBox(L"Failed to SetUp_Model : CObject_Map");
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

HRESULT CObject_Map::SetUp_ObjectsMap(const char* pFilePath)
{
    for (filesystem::recursive_directory_iterator FileIter(pFilePath);
        FileIter != filesystem::end(FileIter); ++FileIter)
    {
        const filesystem::directory_entry& entry = *FileIter;

        if (entry.is_directory())
            continue;

        wstring wstrPath = entry.path().relative_path();
        string strFullPath;
        strFullPath.assign(wstrPath.begin(), wstrPath.end());

        CMap* pMap = CMap::Create(strFullPath.c_str());

        if (!pMap)
            continue;

        pMap->Initialize();
        Add_Child(pMap);
    }

    return S_OK;
}
