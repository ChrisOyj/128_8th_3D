#include "CModel_Renderer.h"

#include "Model.h"
#include "GameObject.h"

#include "CShader.h"
#include "Transform.h"
#include "Render_Manager.h"

#include "MeshContainer.h"


CModel_Renderer::CModel_Renderer(_uint iGroupID)
	: CRenderer(iGroupID)
{
}

CModel_Renderer::~CModel_Renderer()
{
	Release();
}

CModel_Renderer* CModel_Renderer::Create(_uint iGroupID, const RENDER_GROUP& eRenderGroup, const _uint& iCurPass, const _float4& vOffsetPos)
{
	CModel_Renderer* pRenderer = new CModel_Renderer(iGroupID);

	pRenderer->m_iCurPass = iCurPass;
	pRenderer->m_vOffsetPos = vOffsetPos;
	pRenderer->m_eRenderGroup = eRenderGroup;

	if (FAILED(pRenderer->Initialize_Prototype()))
	{
		SAFE_DELETE(pRenderer);
		Call_MsgBox(L"Failed to Initialize_Prototype : CModel_Renderer");
		return nullptr;
	}

	return pRenderer;
}

HRESULT CModel_Renderer::Initialize_Prototype()
{


	return S_OK;
}

HRESULT CModel_Renderer::Initialize()
{
	return S_OK;
}

void CModel_Renderer::Start()
{
	CComponent::Start();
	m_pShaderCom = GET_COMPONENT_FROM(m_pOwner, CShader);
	m_pModelCom = GET_COMPONENT_FROM(m_pOwner, CModel);

	if (m_pModelCom)
	{
		if (m_pModelCom->Get_ModelType() == TYPE_ANIM)
		{
			if (m_iCurPass <= 8)
				m_eShadowType = DEFAULTSHADOW;

		}
	}

	if (m_pShaderCom->Get_ShaderFileIndex() - CUR_ENGINESHADER == 3)
	{
		if (m_eShadowType == DEFAULTSHADOW)
			m_iShadowPass = 8;
		else
			m_iShadowPass = 10;
	}
	else if (m_pShaderCom->Get_ShaderFileIndex() - CUR_ENGINESHADER == 4)
		m_iShadowPass = 7;
	else if (m_pShaderCom->Get_ShaderFileIndex() - CUR_ENGINESHADER == 5)
	{
		if (m_eShadowType == DEFAULTSHADOW)
			m_iShadowPass = 2;
		else if (m_eShadowType == STATICSHADOW)
			m_iShadowPass = 3;
	}
}

void CModel_Renderer::Tick()
{
}

void CModel_Renderer::Late_Tick()
{
	__super::Late_Tick();

	if (m_eShadowType == DEFAULTSHADOW)
		CRender_Manager::Get_Instance()->Add_Renderer(RENDER_SHADOW, this);

}

HRESULT CModel_Renderer::Render()
{
	m_pShaderCom->CallBack_SetRawValues(m_pShaderCom, "");

	_uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NoiseTexture", i, aiTextureType_DIFFUSE_ROUGHNESS)))
			return E_FAIL;

		/*if (FAILED(m_pShaderCom->Begin(m_iCurPass)))
			return E_FAIL;*/

		m_pModelCom->Render(i, m_pShaderCom, m_iCurPass, "g_Bones");
	}

	return S_OK;
}

HRESULT CModel_Renderer::Bake_Shadow(_float4x4 ViewMatrix)
{
	m_pOwner->Get_Transform()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix");

	_uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	//카메라 위치 여기서 만들어주기
	//_float fDistance = 100.f;
	//_float4x4 vLightVewMatrix = ViewMatrix;
	//_float4	vPos = m_pOwner->Get_Transform()->Get_World(WORLD_POS);
	//_float4 vLook = (*((_float4*)&vLightVewMatrix.m[WORLD_LOOK]));
	//vPos -= vLook * fDistance;

	///*Point */
	////vPos = _float4(500.f, 500.f, -500.f)

	//(*((_float4*)&vLightVewMatrix.m[WORLD_POS])) = vPos;
	//vLightVewMatrix.Inverse();
	//vLightVewMatrix.Transpose(); 

	m_pShaderCom->Set_RawValue("g_ViewMatrix", &ViewMatrix, sizeof(_float4x4));

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		m_pModelCom->Bake_Shadow(i, m_pShaderCom, m_iShadowPass);
	}

	return S_OK;

}

void CModel_Renderer::Release()
{
}
