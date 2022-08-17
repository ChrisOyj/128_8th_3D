#include "stdafx.h"
#include "CSkyBox.h"

#include "GameInstance.h"
#include "Transform.h"

#include "CComponent_Factory.h"

#include "CMesh_Cube.h"
#include "CShader.h"
#include "Renderer.h"
#include "Texture.h"

CSkyBox::CSkyBox()
{
}

CSkyBox::~CSkyBox()
{
}

HRESULT CSkyBox::Initialize_Prototype()
{
    CShader* pCubeShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXCUBETEX, VTXCUBETEX_DECLARATION::Element, VTXCUBETEX_DECLARATION::iNumElements);
    pCubeShader->Initialize();
    Add_Component(pCubeShader);
    Add_Component<CMesh>(CComponent_Factory::Clone_Component<CMesh_Cube>(this));
    CRenderer* pRenderer = CRenderer::Create(CP_RENDERER, RENDER_PRIORITY, VTXCUBETEX_PASS_SKYBOX);
    Add_Component(pRenderer);
    
    CTexture* pTexture = CTexture::Create(CP_AFTER_TRANSFORM,
        L"../bin/resources/textures/skybox/sky_%d.dds", 4);
    pTexture->Initialize();
    pTexture->Set_CurTextureIndex(3);
    Add_Component(pTexture);

    return S_OK;
}

HRESULT CSkyBox::Initialize()
{
    return S_OK;
}

void CSkyBox::My_Tick()
{
}

void CSkyBox::My_LateTick()
{
    m_pTransform->Set_World(WORLD_POS, CGameInstance::Get_Instance()->Get_ViewPos());
    m_pTransform->Make_WorldMatrix();
}
