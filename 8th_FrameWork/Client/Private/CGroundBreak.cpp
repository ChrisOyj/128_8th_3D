#include "stdafx.h"
#include "CGroundBreak.h"

#include "HIerarchyNode.h"
#include "GameInstance.h"

#include "Model.h"
#include "Physics.h"
#include "CUnit.h"
#include "CModel_Renderer.h"
#include "CShader.h"


#include "Transform.h"
#include "CComponent_Factory.h"

#include "CCamera_Follow.h"

#include "CUtility_Transform.h"

CGroundBreak::CGroundBreak()
{
}

CGroundBreak::~CGroundBreak()
{
}

CGroundBreak* CGroundBreak::Create(wstring	wstrPath, wstring wstrHashcode)
{
	CGroundBreak* pInstance = new CGroundBreak;

	pInstance->m_wstrPath = wstrPath;
	pInstance->m_hcMyCode = Convert_ToHash(wstrHashcode);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CGroundBreak");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}


HRESULT CGroundBreak::Initialize_Prototype()
{
	m_eShaderType = SHADER_VTXMODEL;
	m_eDisableType = FADE;
	m_fFadeOutStartTime = 3.f;
	m_matTrans = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	m_iPassType = VTXMODEL_PASS_GROUNDBREAK;
	m_vEffectFlag = SH_LIGHT_NOSPEC;
	m_vStartScale = _float4(1.5f, 1.5f, 1.5f, 1.f);
	m_vFadeInTargetScale = _float4(1.5f, 1.5f, 1.5f, 1.f);
	m_vFadeOutTargetScale = _float4(1.5f, 1.5f, 1.5f, 1.f);



	return __super::Initialize_Prototype();
}

HRESULT CGroundBreak::Start()
{
	__super::Start();
	GET_COMPONENT(CRenderer)->Set_RenderGroup(RENDER_NONALPHA);

	return S_OK;
}

void CGroundBreak::OnEnable()
{
	__super::OnEnable();
}
