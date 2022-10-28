#include "..\Public\Renderer.h"
#include "GameInstance.h"
#include "GameObject.h"

#include "Render_Manager.h"

#include "Transform.h"

#include "CShader.h"
#include "Texture.h"
#include "CMesh.h"

CRenderer::CRenderer(_uint iGroupID)
	: CComponent(iGroupID)
	, m_eRenderGroup(RENDER_END)
{
}

CRenderer::CRenderer(const CRenderer& _origin)
	: CComponent(_origin)
	, m_eRenderGroup(_origin.m_eRenderGroup)
	, m_iCurPass(_origin.m_iCurPass)
	, m_vOffsetPos(_origin.m_vOffsetPos)
	, m_vFinalPos(_origin.m_vFinalPos)
{
}


CRenderer::~CRenderer()
{
	Release();
}

CRenderer* CRenderer::Create(_uint iGroupID, const RENDER_GROUP& eRenderGroup, const _uint& iCurPass, const _float4& vOffsetPos)
{
	CRenderer* pRenderer = new CRenderer(iGroupID);

	pRenderer->m_iCurPass = iCurPass;
	pRenderer->m_vOffsetPos = vOffsetPos;
	pRenderer->m_eRenderGroup = eRenderGroup;

	if (FAILED(pRenderer->Initialize_Prototype()))
	{
		SAFE_DELETE(pRenderer);
		Call_MsgBox(L"Failed to Initialize_Prototype : CRenderer");
		return nullptr;
	}

	return pRenderer;
}

_float4 CRenderer::Get_WorldPosition()
{
	return m_pOwner->Get_Transform()->Get_World(WORLD_POS);
}

void CRenderer::Late_Tick()
{
	m_vFinalPos = m_pOwner->Get_Transform()->Get_World(WORLD_POS);
	m_vFinalPos += m_vOffsetPos.MultiplyCoord(m_pOwner->Get_Transform()->Get_WorldMatrix());
	CRender_Manager::Get_Instance()->Add_Renderer(m_eRenderGroup, this);
}

HRESULT CRenderer::Render()
{
	if (!m_pTextureList.empty())
	{
		_uint iIndex = 0;
		for (auto& elem : m_pTextureList)
		{
			string strConstantName;
			if (iIndex == 0)
				strConstantName = "g_DiffuseTexture";
			else if (iIndex == 1)
				strConstantName = "g_NoiseTexture";
			else if (iIndex == 2)
				strConstantName = "g_NormalTexture";

			if (FAILED(m_pShaderCom->SetUp_ShaderResources(elem, strConstantName.c_str())))
				return E_FAIL;

			iIndex++;
		}

		
	}
	else
	{
		int i = 0;
	}
	
	
	m_pShaderCom->CallBack_SetRawValues(m_pShaderCom, "");

	if (FAILED(m_pShaderCom->Begin(m_iCurPass)))
		return E_FAIL;

	if (FAILED(m_pMeshCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Bake_Shadow(_float4x4 ViewMatrix)
{
	//월드매트릭스
	/*m_pOwner->Get_Transform()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix");

	if (FAILED(m_pShaderCom->Begin(m_iShadowPass)))
		return E_FAIL;

	if (FAILED(m_pMeshCom->Render()))
		return E_FAIL;

	Call_MsgBox(L"Not Ready for Shadow");*/

	return S_OK;
}

void CRenderer::Release()
{
}

HRESULT CRenderer::Initialize_Prototype()
{
	

	return S_OK;
}

HRESULT CRenderer::Initialize()
{
	

	return S_OK;
}

void CRenderer::Start()
{
	__super::Start();

	m_pShaderCom = GET_COMPONENT_FROM(m_pOwner, CShader);
	m_pMeshCom = GET_COMPONENT_FROM(m_pOwner, CMesh);

	list<CComponent*> pTextures = m_pOwner->Get_Component<CTexture>();

	for (auto& elem : pTextures)
		m_pTextureList.push_back(static_cast<CTexture*>(elem));
		


}

void CRenderer::Tick()
{
}
