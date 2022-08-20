#include "stdafx.h"
#include "CTestObj.h"

#include "GameInstance.h"

#include "Texture.h"
#include "CShader.h"
#include "Renderer.h"
#include "Transform.h"
#include "Functor.h"
#include "CFader.h"
#include "CMesh_Rect.h"

#include "CComponent_Factory.h"
#include "CPrototype_Factory.h"

CTestObj::CTestObj()
{
}

CTestObj::~CTestObj()
{
}

HRESULT CTestObj::Initialize_Prototype()
{
    Add_Component(CComponent_Factory::Clone_Component<CShader>(this));
    Add_Component<CMesh>(CComponent_Factory::Clone_Component<CMesh_Rect>(this));
    Add_Component(CRenderer::Create(CP_RENDERER, RENDER_NONALPHA, 0, _float4(0.f, 0.f, 0.f)));

    CTexture* pTexture = CTexture::Create(CP_AFTER_TRANSFORM,
        L"../bin/resources/textures/T_ENV_LKNVLD_Ground_03_BC.tga", 1);
    pTexture->Initialize();
    Add_Component(pTexture);

    return S_OK;
}

HRESULT CTestObj::Initialize()
{
    return S_OK;
}
