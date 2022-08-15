#include "stdafx.h"
#include "CLoading_BG.h"

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

CLoading_BG::CLoading_BG()
{
}

CLoading_BG::~CLoading_BG()
{
}

CLoading_BG* CLoading_BG::Create(const _float4& vStartPosition)
{
	CLoading_BG* pInstance = new CLoading_BG;

	if (FAILED(pInstance->SetUp_Components(vStartPosition)))
	{
		Call_MsgBox(L"Failed to SetUp_Components : CLoading_BG");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CLoading_BG");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CLoading_BG::SetUp_Components(const _float4& vStartPosition)
{
	m_pTransform->Set_World(WORLD_POS, CFunctor::OrthoToRealPosition(vStartPosition));
	m_pTransform->Set_Scale(_float4(_float(g_iWinCX), _float(g_iWinCY), 0.5f, 1.f));

	CFader* pFader = Add_Component(CComponent_Factory::Clone_Component<CFader>(this));

	CTexture* pTexture = CTexture::Create(CP_AFTER_TRANSFORM,
		L"../bin/resources/textures/ui/loading_screen/T_UI_LoadingScreen_%.3d_BC.png", 10);

	Add_Component(pTexture);

	FADEDESC	tFadeDesc;
	tFadeDesc.bFadeFlag = FADE_TIME;
	tFadeDesc.eKeyType = KEY::ENTER;
	tFadeDesc.fAlpha = 0.f;
	tFadeDesc.fFadeInTime = 1.5f;
	tFadeDesc.fFadeOutTime = 1.5f;
	tFadeDesc.fFadeOutStartTime = 4.f;
	tFadeDesc.eFadeOutType = FADEDESC::FADEOUT_RANDOMTEXTURE;
	pFader->Get_FadeDesc() = tFadeDesc;

	return S_OK;
}
