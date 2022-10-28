#include "stdafx.h"
#include "CKOLine.h"

#include "HIerarchyNode.h"
#include "GameInstance.h"

#include "Model.h"
#include "Physics.h"
#include "CUnit.h"
#include "CModel_Renderer.h"
#include "CShader.h"
#include "Texture.h"

#include "Transform.h"
#include "CComponent_Factory.h"

#include "CCamera_Follow.h"

#include "CUtility_Transform.h"

CKOLine::CKOLine()
{
}

CKOLine::~CKOLine()
{
}

CKOLine* CKOLine::Create(wstring wstrPath, _float4 vLook)
{
	CKOLine* pInstance = new CKOLine;

	if (FAILED(pInstance->SetUp_Line( wstrPath, vLook)))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CKOLine");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CKOLine");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CKOLine::Initialize_Prototype()
{
	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXEFFECTS,
		VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	CModel_Renderer* pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_ALPHA, VTXEFFECT_PASS_LINEX
		, _float4(0.f, 0.f, 0.f, 1.f));

	Add_Component<CRenderer>(pRenderer);

	
	m_iPassType = VTXEFFECT_PASS_LINEX;
	m_fUVSpeedX = 2.5f;
	m_fFadeOutStartTime = frandom(0.2f, 0.5f);

	m_vEffectFlag = SH_EFFECT_DEFAULT;
	m_vGlowFlag = _float4(1.f, 0.f, 0.f, 1.f);
	

	return S_OK;
}

HRESULT CKOLine::Initialize()
{
	GET_COMPONENT(CModel)->Get_Materials().front().second.pTextures[aiTextureType_DIFFUSE_ROUGHNESS]->Add_Texture(L"../bin/resources/textures/Effects/GradationColor/T_EFF_GMS_ChakraClaw_A_01.png");
	GET_COMPONENT(CModel)->Get_Materials().front().second.pTextures[aiTextureType_DIFFUSE_ROUGHNESS]->Set_CurTextureIndex(1);

	GET_COMPONENT(CModel)->Get_Materials().front().second.pTextures[1]->Add_Texture(L"../bin/resources/textures/effects/gradientMap/T_EFF_Blur_08_M.dds");
	GET_COMPONENT(CModel)->Get_Materials().front().second.pTextures[1]->Set_CurTextureIndex(1);
	

	return S_OK;
}

void CKOLine::My_LateTick()
{
	m_fTimeAcc += fDT;

	if (m_fTimeAcc > m_fFadeOutStartTime)
	{
		m_fCurUVPlusX += m_fUVSpeedX * fDT;

		if (m_fCurUVPlusX >= 1.f)
		{
			DISABLE_GAMEOBJECT(this);
		}
	}

	
}

HRESULT CKOLine::SetUp_Line(wstring wstrPath, _float4 vLook)
{
	m_pTransform->Set_Look(vLook);

	_float4x4 transMat = XMMatrixScaling(0.015f, 0.015f, 0.015f); //* XMMatrixRotationZ(XMConvertToRadians(-45.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	Add_Component(CModel::Create(CP_BEFORE_RENDERER, TYPE_NONANIM, wstrPath.c_str(), transMat));

	return S_OK;
}
