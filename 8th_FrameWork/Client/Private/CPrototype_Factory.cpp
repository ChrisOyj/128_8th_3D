#include "stdafx.h"
#include "CPrototype_Factory.h"

#include "CMesh_Rect.h"
#include "Texture.h"
#include "CShader.h"
#include "Renderer.h"
#include "Transform.h"

#include "GameInstance.h"

#include "CComponent_Factory.h"

#include "Functor.h"

HRESULT CPrototype_Factory::SetUp_DefaultComponents()
{
    if (!CFunctor::Check_Component_Prototype_Exist(CPrototype_Factory::DEFAULT_SHADER))
    {
        CShader* pShader = CShader::Create(CP_END, SHADER_DEFAULT, VTXTEX_DECLARATION::Element,
            VTXTEX_DECLARATION::iNumElements);
        if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(CPrototype_Factory::DEFAULT_SHADER, pShader)))
            return E_FAIL;
    }
   
    if (!CFunctor::Check_Component_Prototype_Exist(CPrototype_Factory::DEFAULT_MESH_RECT))
    {
        CMesh_Rect* pMesh = CMesh_Rect::Create(CP_END);
        if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(CPrototype_Factory::DEFAULT_MESH_RECT, pMesh)))
            return E_FAIL;
    }
    
    if (!CFunctor::Check_Component_Prototype_Exist(CPrototype_Factory::DEFAULT_RENDERER))
    {
        CRenderer* pRenderer = CRenderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXTEX_PASS_DEFAULT, ZERO_VECTOR);
        if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(CPrototype_Factory::DEFAULT_RENDERER, pRenderer)))
            return E_FAIL;
    }

    if (!CFunctor::Check_Component_Prototype_Exist(CPrototype_Factory::DEFAULT_UI_RENDERER))
    {
        CRenderer* pRenderer = CRenderer::Create(CP_RENDERER, RENDER_UI, VTXTEX_PASS_ALPHA, ZERO_VECTOR);
        if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(CPrototype_Factory::DEFAULT_UI_RENDERER, pRenderer)))
            return E_FAIL;
    }
   



    return S_OK;
}
