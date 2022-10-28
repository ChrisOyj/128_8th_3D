#include "stdafx.h"
#include "CCinemaDark.h"

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
#include "CCinemaDark.h"

CCinemaDark::CCinemaDark()
{
}

CCinemaDark::~CCinemaDark()
{
}

CCinemaDark* CCinemaDark::Create(_bool bUp)
{
	CCinemaDark* pInstance = new CCinemaDark;

	_float4 vStartPosition = _float4(0.f, (_float)g_iWinCY / 2.f, 0.f, 1.f);

	if (bUp)
		vStartPosition = _float4(0.f, (_float)g_iWinCY / -2.f, 0.f, 1.f);

	pInstance->m_pTransform->Set_World(WORLD_POS, (vStartPosition));
	pInstance->m_pTransform->Make_WorldMatrix();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CCinemaDark");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CCinemaDark::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	_float4 vScale((_float)g_iWinCX, 160.f, 1.f, 1.f);
	m_pTransform->Set_Scale(vScale);

	CTexture* pTexture = CTexture::Create(CP_AFTER_TRANSFORM,
		L"../bin/resources/textures/Black.png", 1);

	Add_Component(pTexture);
	m_pChildren.clear();

	CFader* pFader = Add_Component(CComponent_Factory::Clone_Component<CFader>(this));

	FADEDESC	tFadeDesc;
	tFadeDesc.bFadeInFlag = FADE_TIME;
	tFadeDesc.bFadeOutFlag = FADE_KEY;
	tFadeDesc.eKeyType = KEY::F8;
	tFadeDesc.fAlpha = 0.f;
	tFadeDesc.fFadeInTime = 0.1f;
	tFadeDesc.fFadeOutTime = 0.1f;
	tFadeDesc.fFadeInStartTime = 0.f;
	tFadeDesc.fFadeOutStartTime = 0.f;
	tFadeDesc.eFadeStyle = FADEDESC::FADE_STYLE_DEFAULT;
	tFadeDesc.eFadeOutType = FADEDESC::FADEOUT_DELETE;
	pFader->Get_FadeDesc() = tFadeDesc;
	pFader->Set_FadeMessage();

	return S_OK;
}

HRESULT CCinemaDark::Initialize()
{
	GET_COMPONENT(CRenderer)->Set_Pass(VTXTEX_PASS_DEFAULT);

	return S_OK;
}
