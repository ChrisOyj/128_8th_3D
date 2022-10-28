#include "stdafx.h"
#include "CWire.h"

#include "HIerarchyNode.h"
#include "GameInstance.h"

#include "Model.h"
#include "Physics.h"
#include "CUnit.h"
#include "Renderer.h"
#include "CShader.h"
#include "CMesh_Cube.h"
#include "Texture.h"

#include "Transform.h"
#include "CComponent_Factory.h"

CWire::CWire()
{
}

CWire::~CWire()
{
}

CWire* CWire::Create(CUnit* pOwner, CHierarchyNode* pRightHand)
{
	CWire* pInstance = new CWire;

	pInstance->m_pOwner = pOwner;
	pInstance->m_pRightHand = pRightHand;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CWire");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CWire::OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos)
{
}

void CWire::Set_ShaderResource(CShader* pShader, const char* name)
{
	pShader->Set_RawValue(name, &m_vEffectFlag, sizeof(_float4));
	pShader->Set_RawValue("g_vGlowFlag", &m_vGlowFlag, sizeof(_float4));
}

void CWire::Reset_Wire(CGameObject* pTargetObj)
{
	m_pTargetObj = pTargetObj;
	m_fTimeAcc = 0.f;
	ENABLE_GAMEOBJECT(this);
}

HRESULT CWire::Initialize_Prototype()
{
	m_pTransform->Make_WorldMatrix();

	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXCUBETEX,
		VTXCUBETEX_DECLARATION::Element, VTXCUBETEX_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	CRenderer* pRenderer = CRenderer::Create(CP_RENDERER, RENDER_ALPHA, VTXCUBETEX_PASS_WIRE
		, _float4(0.f, 0.f, 0.f, 1.f));

	Add_Component<CRenderer>(pRenderer);

	Add_Component<CMesh>(CMesh_Cube::Create(CP_BEFORE_RENDERER));

	m_vEffectFlag = SH_EFFECT_NOBLOOM;
	m_vGlowFlag = GLOW_CHAKRA(0.2f);

	/*CTexture* pTexture = CTexture::Create(CP_AFTER_TRANSFORM,
		L"../bin/resources/textures/skybox/MySkyBox_3.dds", 1);
	pTexture->Initialize();
	Add_Component(pTexture);*/

	return S_OK;
}

HRESULT CWire::Initialize()
{

	return S_OK;
}

HRESULT CWire::Start()
{
	GET_COMPONENT(CShader)->CallBack_SetRawValues += bind(&CWire::Set_ShaderResource, this, placeholders::_1, "g_vFlag");

	return __super::Start();
}

void CWire::My_Tick()
{
	_float4 vHandPos;
	_matrix matRightHand = m_pRightHand->Get_CombinedMatrix();
	
	matRightHand = XMMatrixMultiply(matRightHand, DEFAULT_TRANS_MATRIX);

	_float4x4 matBone;
	XMStoreFloat4x4(&matBone, matRightHand);

	_float4x4 matWorld = m_pOwner->Get_Transform()->Get_WorldMatrix();

	//콤바인 위치 * 월드매트릭스
	vHandPos = (*((_float4*)&matBone.m[WORLD_POS]));
	vHandPos = vHandPos.MultiplyCoord(matWorld);


	_float4 vTargetPos = m_pTargetObj->Get_Transform()->Get_World(WORLD_POS);

	//look
	_float4 vLook = vTargetPos - vHandPos;
	m_pTransform->Set_Look(vLook);

	//scale
	_float	fLength = vLook.Length();
	m_pTransform->Set_Scale(_float4(0.025f, 0.025f, fLength, 1.f));
	//m_pTransform->Set_Scale(_float4(10.f, 10.f, 30.f, 1.f));

	// position
	_float4 vMyPos = XMVectorLerp(vHandPos.XMLoad(), vTargetPos.XMLoad(), 0.5f);
	m_pTransform->Set_World(WORLD_POS, vMyPos);

	m_fTimeAcc += fDT;

	if (m_fTimeAcc > 0.7f)
	{
		if (fLength < 5.f)
		{
			DISABLE_GAMEOBJECT(this);
			return;
		}

		if (m_fTimeAcc > 2.f)
		{
			DISABLE_GAMEOBJECT(this);
			return;
		}

	}

}
