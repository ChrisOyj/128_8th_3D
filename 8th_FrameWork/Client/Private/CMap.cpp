#include "stdafx.h"
#include "CMap.h"

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
#include "MeshContainer.h"

#include "CComponent_Factory.h"
#include "CPrototype_Factory.h"



CMap::CMap()
{
}

CMap::~CMap()
{
}

CMap* CMap::Create(wstring wstrFilePath)
{
    CMap* pInstance = new CMap;

    if (FAILED(pInstance->SetUp_Model(wstrFilePath)))
    {
        Call_MsgBox(L"Failed to SetUp_Model : CMap");
        SAFE_DELETE(pInstance);
        return nullptr;
    }

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Call_MsgBox(L"Failed to Initialize_Prototype : CMap");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CMap::Initialize_Prototype()
{
    CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXMODEL,
        VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElements);
    pShader->Initialize();
    Add_Component(pShader);

    CModel_Renderer* pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXMODEL_PASS_BUILDING
        , _float4(0.f, 0.f, 0.f, 1.f));

    Add_Component<CRenderer>(pRenderer);
    /*CTexture* pTexture = CTexture::Create(CP_AFTER_TRANSFORM,
        L"../bin/resources/textures/terrain/tile%d.dds", 2);
    pTexture->Initialize();
    pTexture->Set_CurTextureIndex(0);
    Add_Component(pTexture);*/

    return S_OK;
}

HRESULT CMap::Initialize()
{
    vector<pair<_uint, CMeshContainer*>>& vecMC = GET_COMPONENT(CModel)->Get_MeshContainers();
    vector<pair<_uint, MODEL_MATERIAL>>& vecMate = GET_COMPONENT(CModel)->Get_Materials();

    for (auto& elem : vecMC)
    {
        elem.second->Set_ShaderFlag(SH_LIGHT_BUILDINGCARTOON);

        //노말 텍스쳐 존재하면
        if (vecMate[elem.second->Get_MaterialIndex()].second.pTextures[6])
        {
            elem.second->Set_CurPass(VTXMODEL_PASS_NORMALMAPPING);
        }

        string strName = vecMate[elem.second->Get_MaterialIndex()].second.strName;
        if (
            strName.find("Windows") != -1 ||
            strName.find("Screen") != -1 ||
            strName.find("Display") != -1)
        {
            elem.second->Set_ShaderFlag(SH_LIGHT_DEFAULT);

            if (strName == "MI_ENV_KNVLLG_AnimatedScreen_01")
            {
                _int iRand = random(1, 3);
                vecMate[elem.second->Get_MaterialIndex()].second.pTextures[0] = vecMate[elem.second->Get_MaterialIndex()].second.pTextures[1];
                vecMate[elem.second->Get_MaterialIndex()].second.pTextures[1] = vecMate[elem.second->Get_MaterialIndex()].second.pTextures[iRand];
                vecMate[elem.second->Get_MaterialIndex()].second.pTextures[iRand] = vecMate[elem.second->Get_MaterialIndex()].second.pTextures[0];
                vecMate[elem.second->Get_MaterialIndex()].second.pTextures[0] = nullptr;
            }
        }

       
    }

    return S_OK;
}

HRESULT CMap::Start()
{
    __super::Start();

    

    return S_OK;
}

HRESULT CMap::SetUp_Model(wstring wstrFilePath)
{
    _float4x4			TransformMatrix;
    TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(0.0f));

    CModel* pModel = CModel::Create(CP_BEFORE_RENDERER, TYPE_NONANIM, wstrFilePath, TransformMatrix);
    
    if (!pModel)
        return E_FAIL;

    pModel->Initialize();
    Add_Component(pModel);

    return S_OK;
}
