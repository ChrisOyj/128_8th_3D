#include "stdafx.h"
#include "CFadeDark.h"

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
#include "CFadeDark.h"

CFadeDark::CFadeDark()
{
}

CFadeDark::~CFadeDark()
{
}

CFadeDark* CFadeDark::Create(_float fFadeInTime, _float fFadeDelayTime, _float fFadeOutTime)
{
	CFadeDark* pInstance = new CFadeDark;

	if (FAILED(pInstance->SetUp_Components(fFadeInTime, fFadeDelayTime, fFadeOutTime)))
	{
		Call_MsgBox(L"Failed to SetUp_Components : CFadeDark");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CFadeDark");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CFadeDark::Initialize()
{
	GET_COMPONENT(CRenderer)->Set_Pass(VTXTEX_PASS_DEFAULT);

	return S_OK;
}



HRESULT CFadeDark::SetUp_Components(_float fFadeInTime, _float fFadeDelayTime, _float fFadeOutTime)
{
	_float4 vStartPosition = _float4((_float)g_iWinCX / 2.f, (_float)g_iWinCY / 2.f, 0.f, 1.f);
	m_pTransform->Set_World(WORLD_POS, CFunctor::OrthoToRealPosition(vStartPosition));
	_float4 vScale((_float)g_iWinCX, (_float)g_iWinCY, 1.f, 1.f);
	m_pTransform->Set_Scale(vScale);

	CTexture* pTexture = CTexture::Create(CP_AFTER_TRANSFORM,
		L"../bin/resources/textures/Black.png", 1);

	Add_Component(pTexture);
	m_pChildren.clear();

	CFader* pFader = Add_Component(CComponent_Factory::Clone_Component<CFader>(this));

	FADEDESC	tFadeDesc;
	tFadeDesc.bFadeInFlag = FADE_KEY;
	tFadeDesc.bFadeOutFlag = FADE_TIME;
	tFadeDesc.eKeyType = KEY::F8;
	tFadeDesc.fAlpha = 0.f;
	tFadeDesc.fFadeInTime = fFadeInTime;
	tFadeDesc.fFadeOutTime = fFadeOutTime;
	tFadeDesc.fFadeInStartTime = 0.f;
	tFadeDesc.fFadeOutStartTime = fFadeDelayTime;
	tFadeDesc.eFadeStyle = FADEDESC::FADE_STYLE_DEFAULT;
	tFadeDesc.eFadeOutType = FADEDESC::FADEOUT_DISABLE;
	pFader->Get_FadeDesc() = tFadeDesc;
	pFader->Set_FadeMessage();

	return S_OK;
}
