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

#include "CDefault_UI.h"

HRESULT CPrototype_Factory::SetUp_DefaultComponents()
{
	if (FAILED(CGameInstance::Get_Instance()->Add_Font(TEXT("Font_Arial"), TEXT("../Bin/Resources/Fonts/125.spriteFont"))))
		return E_FAIL;

	FADEDESC    tFadeDesc;
	tFadeDesc.bFadeFlag = FADE_NONE;
	tFadeDesc.eFadeOutType = FADEDESC::FADEOUT_DELETE;
	tFadeDesc.fAlpha = 1.f;
	tFadeDesc.eKeyType = KEY::ENTER;
	tFadeDesc.fFadeOutTime = 1.f;
	tFadeDesc.fFadeInTime = 1.f;
	tFadeDesc.fFadeOutStartTime = 1.f;

	CFader* pFader = CFader::Create(CP_AFTER_TRANSFORM, tFadeDesc);

	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(pFader)))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(
		CRenderer::Create(CP_RENDERER, RENDER_UI, VTXTEX_PASS_ALPHA, _float4(0.f, 0.f, 0.f))))
		)
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(
		CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXTEX, VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements)
	)))
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(
		CPhysics::Create(CP_AFTER_TRANSFORM)))
		)
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(
		CMesh_Rect::Create(CP_BEFORE_RENDERER)))
		)
		return E_FAIL;

	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(
		CTexture::Create(CP_BEFORE_RENDERER, L"../bin/resources/textures/ui/Jusin_0.png", 1)))
		)
		return E_FAIL;

	return S_OK;
}

HRESULT CPrototype_Factory::SetUp_DefaultGameObjects()
{
	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject_Prototype(
		CDefault_UI::Create()))
	)
		return E_FAIL;

	return S_OK;
}
