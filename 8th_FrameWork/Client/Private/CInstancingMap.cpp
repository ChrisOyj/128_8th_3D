#include "stdafx.h"
#include "CInstancingMap.h"

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



CInstancingMap::CInstancingMap()
{
}

CInstancingMap::~CInstancingMap()
{
}

CInstancingMap* CInstancingMap::Create(wstring wstrFilePath, wstring wstrInstancingPath)
{
    CInstancingMap* pInstance = new CInstancingMap;

    if (FAILED(pInstance->SetUp_Model(wstrFilePath, wstrInstancingPath)))
    {
        Call_MsgBox(L"Failed to SetUp_Model : CInstancingMap");
        SAFE_DELETE(pInstance);
        return nullptr;
    }

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Call_MsgBox(L"Failed to Initialize_Prototype : CInstancingMap");
        SAFE_DELETE(pInstance);
        return nullptr;
    }

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CInstancingMap");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CInstancingMap::Initialize_Prototype()
{
    CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXMODELINSTANCE,
        VTXMODEL_INSTANCE_DECLARATION::Element, VTXMODEL_INSTANCE_DECLARATION::iNumElements);
    pShader->Initialize();
    Add_Component(pShader);

    CModel_Renderer* pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXINSTANCE_PASS_DEFAULT
        , _float4(0.f, 0.f, 0.f, 1.f));
    pRenderer->Set_ShadowType(CModel_Renderer::STATICSHADOW);

    Add_Component<CRenderer>(pRenderer);

    /*CTexture* pTexture = CTexture::Create(CP_AFTER_TRANSFORM,
        L"../bin/resources/textures/terrain/tile%d.dds", 2);
    pTexture->Initialize();
    pTexture->Set_CurTextureIndex(0);
    Add_Component(pTexture);*/
    
    return S_OK;
}

HRESULT CInstancingMap::Initialize()
{
    GET_COMPONENT(CModel)->Set_OutlinePower(0.3f);

    return S_OK;
}

HRESULT CInstancingMap::Start()
{
    __super::Start();



    return S_OK;
}

HRESULT CInstancingMap::SetUp_Model(wstring wstrFilePath, wstring wstrInstancingPath)
{
    _float4x4			TransformMatrix;
    TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(0.0f));
    wstring wstrPath = L"../bin/Instancing/";
    wstrPath += wstrInstancingPath;
    wstrPath += L".dat";
    CModel* pModel = CModel::Create(CP_BEFORE_RENDERER, TYPE_NONANIM, wstrFilePath, wstrPath, TransformMatrix);

    if (!pModel)
        return E_FAIL;

    pModel->Initialize();
    Add_Component(pModel);

    return S_OK;
}
