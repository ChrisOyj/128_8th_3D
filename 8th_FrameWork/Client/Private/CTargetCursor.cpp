#include "stdafx.h"
#include "CTargetCursor.h"

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
#include "CLockedCursor.h"

#include "CUnit.h"

#include "CUser.h"
#include "CUnit_Player.h"

CTargetCursor::CTargetCursor()
{
}

CTargetCursor::~CTargetCursor()
{
}

CTargetCursor* CTargetCursor::Create()
{
	CTargetCursor* pInstance = new CTargetCursor;

	if (FAILED(pInstance->SetUp_Components()))
	{
		Call_MsgBox(L"Failed to SetUp_Components : CTargetCursor");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CTargetCursor");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CTargetCursor::Set_Target(CUnit* pUnit)
{
	//새로 들어왔을 떄
	if (pUnit != m_pTargetUnit)
	{
		//타겟이 없으면 꺼야함
		if (!pUnit)
		{
			GET_COMPONENT(CFader)->Re_FadeOut();
		}
		else
		{
			//다시 켜주기
			GET_COMPONENT(CFader)->Re_FadeIn();
		}

	}

	m_pTargetUnit = pUnit;
}

void CTargetCursor::On_FixedTarget(_bool bActivate)
{
	//다시 켜주기
	GET_COMPONENT(CFader)->Re_FadeIn();
	m_bLocked = bActivate;
	if (bActivate)
	{
		GET_COMPONENT(CTexture)->Set_CurTextureIndex(1);
		ENABLE_GAMEOBJECT(m_pLockedCursor);
		GET_COMPONENT_FROM(m_pLockedCursor, CFader)->Re_FadeIn();

	}
	else
	{
		GET_COMPONENT(CTexture)->Set_CurTextureIndex(0);
		GET_COMPONENT_FROM(m_pLockedCursor, CFader)->Re_FadeOut();

	}
}

void CTargetCursor::Set_ShaderResource(CShader* pShader, const char* pConstantName)
{
	_float4 vColor = (m_bLocked) ? m_vLockedColor : m_vDefaultColor;
	pShader->Set_RawValue(pConstantName, &vColor, sizeof(_float4));
}

HRESULT CTargetCursor::SetUp_Components()
{
	m_pTransform->Set_Scale(_float4(230.f, 230.f, 0.f, 1.f));

	CFader* pFader = Add_Component(CComponent_Factory::Clone_Component<CFader>(this));

	CTexture* pTexture = CTexture::Create(CP_AFTER_TRANSFORM,
		L"../bin/resources/textures/ui/ingame/target/T_UI_TargetCursol_Effect0.png", 1);
	Add_Component(pTexture);

	pTexture->Add_Texture(L"../bin/resources/textures/ui/ingame/target/T_UI_TargetCursol_Effect1.dds");

	CPhysics* pPhysics = CComponent_Factory::Clone_Component<CPhysics>(this);
	Add_Component(pPhysics);
	pPhysics->Set_TurnDir(_float4(0.f, 0.f, 1.f, 0.f));
	pPhysics->Set_TurnSpeed(0.5f);


	FADEDESC	tFadeDesc;
	tFadeDesc.bFadeInFlag = FADE_KEY;
	tFadeDesc.bFadeOutFlag = FADE_KEY;
	tFadeDesc.eKeyType = KEY::F8;
	tFadeDesc.fAlpha = 0.f;
	tFadeDesc.fFadeInTime = 0.4f;
	tFadeDesc.fFadeOutTime = 0.3f;
	tFadeDesc.fFadeInStartTime = 0.f;
	tFadeDesc.fFadeOutStartTime = 0.f;
	tFadeDesc.eFadeStyle = FADEDESC::FADE_STYLE_SCALEUP;
	tFadeDesc.eFadeOutType = FADEDESC::FADEOUT_NONE;
	pFader->Get_FadeDesc() = tFadeDesc;

	return S_OK;
}

HRESULT CTargetCursor::Initialize()
{
	__super::Initialize();

	m_pLockedCursor = CLockedCursor::Create(this);
	m_pLockedCursor->Initialize();
	CREATE_STATIC(m_pLockedCursor, HASHCODE(CLockedCursor));
	DISABLE_GAMEOBJECT(m_pLockedCursor);

	GET_COMPONENT(CRenderer)->Set_Pass(VTXTEX_PASS_COLORNOBLACK);
	m_vDefaultColor = _float4(0.5f, 1.f, 0.5f, 1.f);
	m_vLockedColor = _float4(1.f, 0.3f, 0.3f, 1.f);

	GET_COMPONENT(CShader)->CallBack_SetRawValues +=
		bind(&CTargetCursor::Set_ShaderResource, this, placeholders::_1, "g_vColor");

	return S_OK;
}

void CTargetCursor::My_LateTick()
{
	if (!m_pTargetUnit)
		return;

	if (!m_pTargetUnit->Is_Valid())
	{
		//Set_Target(nullptr);

		if (m_bLocked)
			PLAYER->On_FixedTarget(false);

		//Locked녀석의 fade만 컨트롤해야함
		return;
	}




	//유닛의 투영위치 구하기
	_float4 vTargetWorldPos = m_pTargetUnit->Get_Transform()->Get_World(WORLD_POS);
	vTargetWorldPos.y += 0.7f;

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
			ENABLE_COMPONENT(GET_COMPONENT(CRenderer));
	}
	//ENABLE_COMPONENT(GET_COMPONENT(CRenderer));

	_float4x4 matVP = GAMEINSTANCE->Get_CurViewMatrix() * GAMEINSTANCE->Get_CurProjMatrix();

	vTargetWorldPos = vTargetWorldPos.MultiplyCoord(matVP);

	vTargetWorldPos.x *= g_iWinCX * 0.5f;
	vTargetWorldPos.y *= g_iWinCY * 0.5f;
	vTargetWorldPos.z = 0.1f;
	vTargetWorldPos.w = 1.f;
	m_pTransform->Set_World(WORLD_POS, vTargetWorldPos);
	m_pTransform->Make_WorldMatrix();



}

void CTargetCursor::OnEnable()
{
	__super::OnEnable();
	ENABLE_GAMEOBJECT(m_pLockedCursor);
	GET_COMPONENT(CFader)->Get_FadeDesc().fAlpha = 0.f;
	GET_COMPONENT(CFader)->Get_FadeState() = CFader::FADEINREADY;

}

void CTargetCursor::OnDisable()
{
	__super::OnDisable();
	m_pTargetUnit = nullptr;
	GET_COMPONENT(CFader)->Get_FadeState() = CFader::FADEINREADY;
	GET_COMPONENT(CFader)->Get_FadeDesc().fAlpha = 0.f;
	DISABLE_GAMEOBJECT(m_pLockedCursor);
}
