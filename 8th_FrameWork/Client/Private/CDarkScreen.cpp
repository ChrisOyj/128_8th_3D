#include "stdafx.h"
#include "CDarkScreen.h"

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

CDarkScreen::CDarkScreen()
{
}

CDarkScreen::~CDarkScreen()
{
}

CDarkScreen* CDarkScreen::Create(const _float4& vStartPosition, const _float4& vScale)
{
	CDarkScreen* pInstance = new CDarkScreen;

	if (FAILED(pInstance->SetUp_Components(vStartPosition, vScale)))
	{
		Call_MsgBox(L"Failed to SetUp_Components : CDarkScreen");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CDarkScreen");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CDarkScreen::Initialize()
{
	GET_COMPONENT(CRenderer)->Set_Pass(VTXTEX_PASS_DEFAULT);

	return S_OK;
}

HRESULT CDarkScreen::SetUp_Components(const _float4& vStartPosition, const _float4& vScale)
{
	m_pTransform->Set_World(WORLD_POS, CFunctor::OrthoToRealPosition(vStartPosition));
	m_pTransform->Set_Scale(vScale);

	CTexture* pTexture = CTexture::Create(CP_AFTER_TRANSFORM,
		L"../bin/resources/textures/Black.png", 1);

	Add_Component(pTexture);
	m_pChildren.clear();

	return S_OK;
}
