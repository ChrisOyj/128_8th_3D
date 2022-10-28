#include "stdafx.h"
#include "CLockedCursor.h"

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
#include "CTargetCursor.h"
#include "CUnit.h"

#include "CUnit_Player.h"
#include "CUser.h"
CLockedCursor::CLockedCursor()
{
}

CLockedCursor::~CLockedCursor()
{
}

CLockedCursor* CLockedCursor::Create(CTargetCursor* pTargetCursor)
{
	CLockedCursor* pInstance = new CLockedCursor;

	pInstance->m_pTargetCursor = pTargetCursor;

	if (FAILED(pInstance->SetUp_Components()))
	{
		Call_MsgBox(L"Failed to SetUp_Components : CLockedCursor");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CLockedCursor");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CLockedCursor::Set_ShaderResource(CShader* pShader, const char* pConstantName)
{
	_float4 vColor = m_vDefaultColor;
	pShader->Set_RawValue(pConstantName, &vColor, sizeof(_float4));
}

HRESULT CLockedCursor::SetUp_Components()
{
	m_pTransform->Set_Scale(_float4(245.f, 245.f, 0.f, 1.f));

	CFader* pFader = Add_Component(CComponent_Factory::Clone_Component<CFader>(this));

	CTexture* pTexture = CTexture::Create(CP_AFTER_TRANSFORM,
		L"../bin/resources/textures/ui/ingame/target/T_UI_TargetCursol_Lock3.png", 1);
	Add_Component(pTexture);

	CPhysics* pPhysics = CComponent_Factory::Clone_Component<CPhysics>(this);
	Add_Component(pPhysics);
	pPhysics->Set_TurnDir(_float4(0.f, 0.f, -1.f, 0.f));
	pPhysics->Set_TurnSpeed(0.3f);


	FADEDESC	tFadeDesc;
	tFadeDesc.bFadeInFlag = FADE_KEY;
	tFadeDesc.bFadeOutFlag = FADE_KEY;
	tFadeDesc.eKeyType = KEY::F8;
	tFadeDesc.fAlpha = 0.f;
	tFadeDesc.fFadeInTime = 1.5f;
	tFadeDesc.fFadeOutTime = 0.3f;
	tFadeDesc.fFadeInStartTime = 0.f;
	tFadeDesc.fFadeOutStartTime = 0.f;
	tFadeDesc.eFadeStyle = FADEDESC::FADE_STYLE_SCALEUP;
	tFadeDesc.eFadeOutType = FADEDESC::FADEOUT_DISABLE;
	pFader->Get_FadeDesc() = tFadeDesc;

	return S_OK;
}

HRESULT CLockedCursor::Initialize()
{
	__super::Initialize();

	GET_COMPONENT(CRenderer)->Set_Pass(VTXTEX_PASS_COLORNOBLACK);
	m_vDefaultColor = _float4(1.f, 0.3f, 0.3f, 1.f);

	GET_COMPONENT(CShader)->CallBack_SetRawValues +=
		bind(&CLockedCursor::Set_ShaderResource, this, placeholders::_1, "g_vColor");

	return S_OK;
}

void CLockedCursor::My_LateTick()
{
	if (!m_pTargetCursor)
		return;

	if (!m_pTargetCursor->Is_Valid())
	{
		return;
	}




	//유닛의 투영위치 구하기
	_float4 vTargetWorldPos = m_pTargetCursor->Get_Transform()->Get_World(WORLD_POS);

	if (!GAMEINSTANCE->isIn_Frustum_InWorldSpace(vTargetWorldPos.XMLoad(), 0.3f))
	{
		if (!GET_COMPONENT(CRenderer)->Is_Disable())
		{
			DISABLE_COMPONENT(GET_COMPONENT(CRenderer));
		}

		return;
	}
	else
	{
		if (GET_COMPONENT(CRenderer)->Is_Disable())
		{
			if (PLAYER->Is_LockedTarget())
				ENABLE_COMPONENT(GET_COMPONENT(CRenderer));

		}
	}


	vTargetWorldPos.z += 0.1f;
	m_pTransform->Set_World(WORLD_POS, vTargetWorldPos);
	m_pTransform->Make_WorldMatrix();



}
