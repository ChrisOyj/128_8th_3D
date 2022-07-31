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

CRenderer* CRenderer::Create(_uint iGroupID, const _uint& iCurPass)
{
	CRenderer* pRenderer = new CRenderer(iGroupID);

	pRenderer->m_iCurPass = iCurPass;

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
	CRender_Manager::Get_Instance()->Add_Renderer(m_eRenderGroup, this);
}

HRESULT CRenderer::Render()
{
	if (FAILED(m_pShaderCom->SetUp_ShaderResources(m_iCurPass, m_pTextureCom)))
		return E_FAIL;

	if (FAILED(m_pMeshCom->Render()))
		return E_FAIL;

	return S_OK;
}

void CRenderer::Release()
{
}

HRESULT CRenderer::Initialize_Prototype()
{
	if (!m_pMeshCom || !m_pShaderCom || !m_pTextureCom)
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Initialize()
{
	m_pShaderCom = m_pOwner->Get_Component<CShader>()[0];
	m_pMeshCom = m_pOwner->Get_Component<CMesh>()[0];
	m_pTextureCom = m_pOwner->Get_Component<CTexture>()[0];

	return S_OK;
}

void CRenderer::Tick()
{
}
