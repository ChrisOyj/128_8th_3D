#include "stdafx.h"
#include "CTerrain.h"

#include "GameInstance.h"

#include "Texture.h"
#include "CShader.h"
#include "Renderer.h"
#include "Transform.h"
#include "Functor.h"
#include "CFader.h"
#include "CMesh_Rect.h"
#include "CMesh_Terrain.h"

#include "CComponent_Factory.h"
#include "CPrototype_Factory.h"

CTerrain::CTerrain()
{
}

CTerrain::~CTerrain()
{
}

HRESULT CTerrain::Initialize_Prototype()
{
    CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXNOR,
        VTXNORTEX_DECLARATION::Element, VTXNORTEX_DECLARATION::iNumElements);
    pShader->Initialize();
    Add_Component(pShader);


    CMesh_Terrain* pTerrain = CMesh_Terrain::Create(CP_BEFORE_RENDERER, 100, 100);
    pTerrain->Initialize();
    Add_Component<CMesh>(pTerrain);

    Add_Component(CRenderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXNOR_PASS_TERRAIN));

    CTexture* pTexture = CTexture::Create(CP_AFTER_TRANSFORM,
        L"../bin/resources/textures/terrain/tile%d.dds", 2);
    pTexture->Initialize();
    pTexture->Set_CurTextureIndex(0);
    Add_Component(pTexture);

    return S_OK;
}

HRESULT CTerrain::Initialize()
{
    return S_OK;
}
