#include "stdafx.h"
#include "CPrototype_Factory.h"

#include "CMesh_Rect.h"
#include "Texture.h"
#include "CShader.h"
#include "Renderer.h"
#include "Transform.h"
#include "Physics.h"
#include "CFader.h"

#include "GameInstance.h"

#include "CComponent_Factory.h"

#include "Functor.h"

HRESULT CPrototype_Factory::SetUp_DefaultComponents()
{
    if (!CFunctor::Check_Component_Prototype_Exist(CPrototype_Factory::DEFAULT_SHADER))
    {
        CShader* pShader = CShader::Create(CP_END, SHADER_VTXTEX, VTXTEX_DECLARATION::Element,
            VTXTEX_DECLARATION::iNumElements);

        pShader->Set_ComponentID(COM_SHADER);

        if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(CPrototype_Factory::DEFAULT_SHADER, pShader)))
            return E_FAIL;
    }
   
    if (!CFunctor::Check_Component_Prototype_Exist(CPrototype_Factory::DEFAULT_MESH_RECT))
    {
        CMesh_Rect* pMesh = CMesh_Rect::Create(CP_END);
        pMesh->Set_MeshType(MESH_RECT);
        pMesh->Set_ComponentID(COM_MESH);
        if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(CPrototype_Factory::DEFAULT_MESH_RECT, pMesh)))
            return E_FAIL;
    }
    
    if (!CFunctor::Check_Component_Prototype_Exist(CPrototype_Factory::DEFAULT_RENDERER))
    {
        CRenderer* pRenderer = CRenderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXTEX_PASS_DEFAULT, ZERO_VECTOR);
        pRenderer->Set_ComponentID(COM_RENDERER);

        if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(CPrototype_Factory::DEFAULT_RENDERER, pRenderer)))
            return E_FAIL;
    }

    if (!CFunctor::Check_Component_Prototype_Exist(CPrototype_Factory::DEFAULT_UI_RENDERER))
    {
        CRenderer* pRenderer = CRenderer::Create(CP_RENDERER, RENDER_UI, VTXTEX_PASS_ALPHA, ZERO_VECTOR);
        pRenderer->Set_ComponentID(COM_RENDERER);

        if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(CPrototype_Factory::DEFAULT_UI_RENDERER, pRenderer)))
            return E_FAIL;
    }

    if (!CFunctor::Check_Component_Prototype_Exist(CPrototype_Factory::DEFAULT_PHYSICS))
    {
        CPhysics* pPhysics = CPhysics::Create(CP_AFTER_TRANSFORM);
        pPhysics->Set_ComponentID(COM_PHYSICS);

        if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(CPrototype_Factory::DEFAULT_PHYSICS, pPhysics)))
            return E_FAIL;
    }
   
    if (!CFunctor::Check_Component_Prototype_Exist(CPrototype_Factory::DEFAULT_TEXTURE))
    {
        CTexture* pTexture = CTexture::Create(CP_END, L"../bin/resources/textures/ui/Jusin_0.png", 1);
        pTexture->Set_ComponentID(COM_TEXTURE);
        if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(CPrototype_Factory::DEFAULT_TEXTURE, pTexture)))
            return E_FAIL;
    }

    if (!CFunctor::Check_Component_Prototype_Exist(CPrototype_Factory::DEFAULT_FADER))
    {
        FADEDESC    tFadeDesc;
        tFadeDesc.bFadeFlag = FADE_NONE;
        tFadeDesc.eFadeOutType = FADEDESC::FADEOUT_DELETE;
        tFadeDesc.fAlpha = 1.f;
        tFadeDesc.eKeyType = KEY::ENTER;
        tFadeDesc.fFadeOutTime = 1.f;
        tFadeDesc.fFadeInTime = 1.f;
        tFadeDesc.fFadeOutStartTime = 1.f;

        CFader* pFader = CFader::Create(CP_AFTER_TRANSFORM, tFadeDesc);
        pFader->Set_ComponentID(COM_FADER);

        if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(CPrototype_Factory::DEFAULT_FADER, pFader)))
            return E_FAIL;
    }


    return S_OK;
}
