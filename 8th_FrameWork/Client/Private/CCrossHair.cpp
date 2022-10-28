#include "stdafx.h"
#include "CCrossHair.h"

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

CCrossHair::CCrossHair()
{
}

CCrossHair::~CCrossHair()
{
}

CCrossHair* CCrossHair::Create(const _float4& vStartPosition)
{
	CCrossHair* pInstance = new CCrossHair;

	if (FAILED(pInstance->SetUp_Components(vStartPosition)))
	{
		Call_MsgBox(L"Failed to SetUp_Components : CCrossHair");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CCrossHair");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CCrossHair::SetUp_Components(const _float4& vStartPosition)
{
	m_pTransform->Set_World(WORLD_POS, CFunctor::OrthoToRealPosition(vStartPosition));
	m_pTransform->Set_Scale(_float4(300.f, 300.f, 0.f, 1.f));

	CFader* pFader = Add_Component(CComponent_Factory::Clone_Component<CFader>(this));

	CTexture* pTexture = CTexture::Create(CP_AFTER_TRANSFORM,
		L"../bin/resources/textures/Crosshair/CrossHair_%d.png", 2);

	CPhysics* pPhysics = CComponent_Factory::Clone_Component<CPhysics>(this);
	Add_Component(pPhysics);
	pPhysics->Set_TurnDir(_float4(0.f, 0.f, -1.f, 0.f));
	pPhysics->Set_TurnSpeed(0.7f);

	Add_Component(pTexture);
	pTexture->Set_CurTextureIndex(1);

	FADEDESC	tFadeDesc;
	tFadeDesc.bFadeInFlag = FADE_KEY;
	tFadeDesc.bFadeOutFlag = FADE_KEY;
	tFadeDesc.eKeyType = KEY::F8;
	tFadeDesc.fAlpha = 0.f;
	tFadeDesc.fFadeInTime = 0.15f;
	tFadeDesc.fFadeOutTime = 0.15f;
	tFadeDesc.fFadeInStartTime = 0.f;
	tFadeDesc.fFadeOutStartTime = 0.f;
	tFadeDesc.eFadeStyle = FADEDESC::FADE_STYLE_SCALEUP;
	tFadeDesc.eFadeOutType = FADEDESC::FADEOUT_NONE;
	pFader->Get_FadeDesc() = tFadeDesc;

	return S_OK;
}

HRESULT CCrossHair::Initialize()
{
	__super::Initialize();

	GET_COMPONENT(CRenderer)->Set_Pass(VTXTEX_PASS_CROSSHAIR);
	return S_OK;
}
