#include "CModel_Renderer.h"

#include "Model.h"
#include "GameObject.h"

#include "CShader.h"

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

	/*if (FAILED(pRenderer->Initialize_Prototype()))
	{
		SAFE_DELETE(pRenderer);
		Call_MsgBox(L"Failed to Initialize_Prototype : CModel_Renderer");
		return nullptr;
	}*/

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
}

void CModel_Renderer::Tick()
{
}

void CModel_Renderer::Late_Tick()
{
	__super::Late_Tick();
}

HRESULT CModel_Renderer::Render()
{
	m_pShaderCom->CallBack_SetRawValues(m_pShaderCom, "");

	

	_uint iNumMeshContainers = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		/*if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;*/

		if (FAILED(m_pShaderCom->Begin(m_iCurPass)))
			return E_FAIL;

		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CModel_Renderer::Release()
{
}
