#include "stdafx.h"
#include "CEffect_Text.h"

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

CEffect_Text::CEffect_Text()
{
}

CEffect_Text::~CEffect_Text()
{
}

CEffect_Text* CEffect_Text::Create(wstring wstrFBXPath, _hashcode hcCode)
{
	CEffect_Text* pInstance = new CEffect_Text;

	pInstance->m_wstrPath = wstrFBXPath;
	pInstance->m_hcMyCode = hcCode;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CEffect_Text");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}


HRESULT CEffect_Text::Initialize_Prototype()
{
	m_matTrans = XMMatrixScaling(0.02f, 0.02f, 0.02f);
	m_iPassType = VTXEFFECT_PASS_TEXT;

	m_vStartScale = _float4(0.1f, 0.1f, 0.1f, 1.f);
	m_vTargetScale = _float4(1.f, 1.f, 1.f, 1.f);
	m_vOverScale = _float4(1.2f, 1.2f, 1.2f, 1.f);
	m_vEffectFlag = SH_EFFECT_DEFAULT;
	m_vGlowFlag = _float4(1.f, 1.f, 1.f, 1.f);

	m_bEffectFlag |= EFFECT_BILLBOARD;

	return __super::Initialize_Prototype();
}

HRESULT CEffect_Text::Start()
{
	__super::Start();

	GET_COMPONENT(CRenderer)->Set_RenderGroup(RENDER_ALPHA);
	//GET_COMPONENT(CModel)->Set_ShaderFlag(SH_LIGHT_NONE);

	m_pTransform->Set_Scale(m_vStartScale);

	return S_OK;
}

void CEffect_Text::My_Tick()
{
	//커졌다가 다시 작아져
	m_fTimeAcc += fDT;


	m_pTransform->Set_Look(GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK) * 1.f);

	_float4 vCurScale;
		_float fRatio = min(m_fTimeAcc / 0.2f, 1.f);
		 vCurScale = XMVectorLerp(m_vStartScale.XMLoad(), m_vOverScale.XMLoad(), fRatio);

	m_pTransform->Set_Scale(vCurScale);

	if (m_fTimeAcc >= 1.2f)
		DISABLE_GAMEOBJECT(this);

	
}

void CEffect_Text::OnEnable()
{
	__super::OnEnable();
	m_pTransform->Set_Scale(m_vStartScale);
}
