#include "stdafx.h"
#include "CDecalEffect.h"

#include "HIerarchyNode.h"
#include "GameInstance.h"

#include "Model.h"
#include "Physics.h"
#include "CUnit.h"
#include "CModel_Renderer.h"
#include "CShader.h"

#include "CCollider_Sphere.h"
#include "Transform.h"
#include "CComponent_Factory.h"
#include "CNavigation.h"

#include "CTrailEffect.h"
#include "CTrailBuffer.h"

#include "CEffects_Factory.h"
#include "MeshContainer.h"

#include "CHit_Ground.h"
#include "CState_Manager.h"
#include "CScript_FollowCam.h"
#include "CUser.h"

#include "CMesh_Cube.h"

#include "Texture.h"

CDecalEffect::CDecalEffect()
{
}

CDecalEffect::~CDecalEffect()
{
}

CDecalEffect* CDecalEffect::Create(wstring DecalImagePath, wstring ColorMapPath, wstring wstrCode)
{
	CDecalEffect* pInstance = new CDecalEffect;

	pInstance->m_wstrMaskMapPath = DecalImagePath;
	pInstance->m_wstrColorMapPath = ColorMapPath;
	pInstance->m_hcMyCode = Convert_ToHash(wstrCode);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CDecalEffect");
		SAFE_DELETE(pInstance);
	}

	return pInstance;

	return nullptr;
}

void CDecalEffect::Set_ShaderResourceAlpha(CShader* pShader, const char* pConstantName)
{
	__super::Set_ShaderResourceAlpha(pShader, pConstantName);


	_float4x4 ProjMat = GAMEINSTANCE->Get_CurProjMatrix();
	_float4x4 ViewMat = GAMEINSTANCE->Get_CurViewMatrix();
	_float4x4 WorldMat = m_pTransform->Get_WorldMatrix();

	

	pShader->Set_RawValue("g_ProjMatrixInv", &ProjMat.Inverse().Transpose(), sizeof(_float4x4));
	pShader->Set_RawValue("g_ViewMatrixInv", &ViewMat.Inverse().Transpose(), sizeof(_float4x4));
	pShader->Set_RawValue("g_WorldMatrixInv", &WorldMat.Inverse().Transpose(), sizeof(_float4x4));


}

HRESULT CDecalEffect::Start()
{
	CGameObject::Start();

	GET_COMPONENT(CShader)->CallBack_SetRawValues += bind(&CDecalEffect::Set_ShaderResourceAlpha, this, placeholders::_1, "g_fAlpha");
	GET_COMPONENT(CShader)->CallBack_SetRawValues += bind(&CEffect::Set_ShaderResourceGlowFlag, this, placeholders::_1, "g_vGlowFlag");


	return S_OK;
}

void CDecalEffect::My_Tick()
{
}

void CDecalEffect::My_LateTick()
{
	__super::My_LateTick();
}

void CDecalEffect::OnEnable()
{
	

	__super::OnEnable();
}

void CDecalEffect::OnDisable()
{
	__super::OnDisable();
}

HRESULT CDecalEffect::Initialize_Prototype()
{
	m_eDisableType = FADE;

	m_fFadeOutStartTime = 2.f;
	m_fFadeOutTime = 0.5f;

	m_hcMyCode = HASHCODE(CDecalEffect);
	m_bEffectFlag = 0;

	m_vFadeInTargetScale = _float4(10.f, 10.f, 10.f, 1.f);
	m_vFadeOutTargetScale = _float4(10.f, 10.f, 10.f, 1.f);


	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXDECALCUBE,
		VTXDECALCUBE_DECLARATION::Element, VTXDECALCUBE_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	CRenderer* pRenderer = CRenderer::Create(CP_RENDERER, RENDER_SSD, VTXDECALCUBE_PASS_DEFAULT
		, _float4(0.f, 0.f, 0.f, 1.f));

	Add_Component<CRenderer>(pRenderer);


	//큐브
	Add_Component<CMesh>(CMesh_Cube::Create_DecalBox(0));

	//텍스쳐
	if (m_wstrMaskMapPath.empty())
		m_wstrMaskMapPath = L"../bin/resources/textures/T_EFF_Gradation_10_M.png";

	Add_Component<CTexture>(CTexture::Create(0, m_wstrMaskMapPath.c_str(), 1));


	if (m_wstrColorMapPath.empty())
		m_wstrColorMapPath = L"../bin/resources/textures/White.png";

	Add_Component<CTexture>(CTexture::Create(0, m_wstrColorMapPath.c_str(), 1));


	return S_OK;
}

HRESULT CDecalEffect::Initialize()
{
	m_vEffectFlag = SH_EFFECT_NOGLOW;

	return S_OK;
}
