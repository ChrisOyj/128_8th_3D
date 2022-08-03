#include "stdafx.h"
#include "CTestObj.h"

#include "CMesh_Rect.h"
#include "Texture.h"
#include "CShader.h"
#include "Renderer.h"
#include "Transform.h"

#include "GameInstance.h"

#include "CComponent_Factory.h"

CTestObj::CTestObj()
{
}

CTestObj::~CTestObj()
{
}

HRESULT CTestObj::Initialize_Prototype()
{
    CTexture* pTexture = CTexture::Create(CP_END, L"../bin/resources/textures/T_EFF_Gradation_10_M.png", 1);
    if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(TEXTURE_PROTOTYPE_ID, pTexture)))
        return E_FAIL;
    Add_Component(CComponent_Factory::Create_FromPrototype(TEXTURE_PROTOTYPE_ID, this));

    CShader* pShader = CShader::Create(CP_END, L"../bin/ShaderFiles/Shader_VtxTex.hlsl", VTXTEX_DECLARATION::Element,
        VTXTEX_DECLARATION::iNumElements);
    if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(100005, pShader)))
        return E_FAIL;

    Add_Component(CComponent_Factory::Create_FromPrototype(100005, this));

    CMesh_Rect* pMesh = CMesh_Rect::Create(CP_END);
    if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(MESHRECT_PROTOTYPE_ID, pMesh)))
        return E_FAIL;
    
    Add_Component(CComponent_Factory::Create_FromPrototype(MESHRECT_PROTOTYPE_ID, this));

    CRenderer* pRenderer = CRenderer::Create(CP_RENDERER, RENDER_NONALPHA, 0, ZERO_VECTOR);
   if (FAILED( CGameInstance::Get_Instance()->Add_Component_Prototype(RENDERER_PROTOTYPE_ID, pRenderer)))
       return E_FAIL;

    Add_Component(CComponent_Factory::Create_FromPrototype(RENDERER_PROTOTYPE_ID, this));

    return S_OK;
}

HRESULT CTestObj::Initialize()
{
    m_pTransform->Set_World(WORLD_POS, _float4(0.f, 0.f, 10.f, 1.f));
    m_pTransform->Set_Scale(_float4(10.f, 10.f, 10.f, 1.f));

    return S_OK;
}
