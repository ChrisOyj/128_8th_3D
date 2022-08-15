#include "stdafx.h"
#include "CLoading_Bar.h"

#include "GameInstance.h"

#include "Texture.h"
#include "CShader.h"
#include "Renderer.h"
#include "Transform.h"
#include "Functor.h"
#include "CFader.h"
#include "CMesh_Rect.h"
#include "Physics.h"

#include "CComponent_Factory.h"
#include "CPrototype_Factory.h"

CLoading_Bar::CLoading_Bar()
{
}

CLoading_Bar::~CLoading_Bar()
{
}

CLoading_Bar* CLoading_Bar::Create(const _float4& vStartPosition)
{
	CLoading_Bar* pInstance = new CLoading_Bar;

	if (FAILED(pInstance->SetUp_Components(vStartPosition)))
	{
		Call_MsgBox(L"Failed to SetUp_Components : CLoading_Bar");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CLoading_Bar");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CLoading_Bar::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	GET_COMPONENT(CRenderer)->Set_Pass(VTXTEX_PASS_LOADINGBAR);


	return S_OK;
}

HRESULT CLoading_Bar::SetUp_Components(const _float4& vStartPosition)
{
	m_pTransform->Set_World(WORLD_POS, CFunctor::OrthoToRealPosition(vStartPosition));
	m_pTransform->Set_Scale(_float4(g_iWinCX * 0.7f, 5.f, 0.f, 1.f));

	CTexture* pTexture = CTexture::Create(CP_AFTER_TRANSFORM,
		L"../bin/resources/textures/ui/loading_screen/T_UI_LoadIndicator_Base_BC.png", 1);

	Add_Component(pTexture);

	return S_OK;
}
