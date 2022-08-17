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

#include "CComponent_Factory.h"
#include "CPrototype_Factory.h"



CMap::CMap()
{
}

CMap::~CMap()
{
}

HRESULT CMap::Initialize_Prototype()
{
    CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXMODEL,
        VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElements);
    pShader->Initialize();
    Add_Component(pShader);

    _float4x4			TransformMatrix;
    TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));

    CModel* pModel = CModel::Create(CP_BEFORE_RENDERER, TYPE_NONANIM, "../bin/resources/meshes/test/",
        "SK_CHR_Sasuke.fbx", TransformMatrix);
    pModel->Initialize();
    Add_Component(pModel);

    CModel_Renderer* pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXMODEL_PASS_DEFAULT
        , _float4(0.f, 0.f, 0.f, 1.f));
    Add_Component<CModel_Renderer>(pRenderer);

    /*CTexture* pTexture = CTexture::Create(CP_AFTER_TRANSFORM,
        L"../bin/resources/textures/terrain/tile%d.dds", 2);
    pTexture->Initialize();
    pTexture->Set_CurTextureIndex(0);
    Add_Component(pTexture);*/

    return S_OK;
}

HRESULT CMap::Initialize()
{
    return S_OK;
}
