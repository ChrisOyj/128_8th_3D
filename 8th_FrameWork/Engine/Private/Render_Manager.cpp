#include "..\Public\Render_Manager.h"

#include "GameObject.h"
#include "Transform.h"
#include "GameInstance.h"
#include "Renderer.h"

#include "Camera_Manager.h"

IMPLEMENT_SINGLETON(CRender_Manager)


CRender_Manager::CRender_Manager()
{
}

HRESULT CRender_Manager::Add_Renderer(RENDER_GROUP eGroup, CRenderer* pRenderer)
{
	if (eGroup >= RENDER_END ||
		nullptr == pRenderer)
		return E_FAIL;

	if (eGroup == RENDER_ALPHA)
	{
		m_pAlphaRenderers.push_back(make_pair(0.f, pRenderer));
		return S_OK;
	}

	m_Renderers[eGroup].push_back(pRenderer);

	return S_OK;
}

HRESULT CRender_Manager::Render()
{
	Sort_AlphaList();

	CCamera_Manager::Get_Instance()->SetUp_ShaderResources();

	for (_uint i = RENDER_PRIORITY; i < RENDER_UI; ++i)
	{
		if (FAILED(Render_Group((RENDER_GROUP)i)))
			return E_FAIL;
	}

	/* UI */

	CCamera_Manager::Get_Instance()->SetUp_ShaderResources(true);

	if (FAILED(Render_Group(RENDER_UI)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRender_Manager::Render_Group(RENDER_GROUP eGroup)
{
	for (auto& elem : m_Renderers[eGroup])
		if (FAILED(elem->Render()))
			return E_FAIL;

	m_Renderers[eGroup].clear();

	return S_OK;
}

void CRender_Manager::Sort_AlphaList()
{
	_float4 vCamPos = CCamera_Manager::Get_Instance()->Get_ViewPos();

	for (auto& elem : m_pAlphaRenderers)
	{
		_float4 vMyPos = elem.second->Get_WorldPosition();
		_float fDistance = (vMyPos - vCamPos).Length();
		elem.first = fDistance;
	}

	m_pAlphaRenderers.sort(greater<pair<_float, CRenderer*>>());
}