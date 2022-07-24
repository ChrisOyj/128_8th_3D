#include "..\Public\Render_Manager.h"

#include "MeshRenderer.h"
#include "UIRenderer.h"
#include "FontRenderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "Shader_Manager.h"
#include "GameInstance.h"

CRender_Manager::CRender_Manager()
{
}

void CRender_Manager::Set_CurLightDir(_float3 vLightDir)
{
	CShader_Manager::Get_Instance()->Set_CurLightDir(vLightDir);
}

HRESULT CRender_Manager::Add_Renderer(RENDERGROUP eGroup, CRenderer* pRenderer)
{
	if (eGroup >= RENDER_END ||
		nullptr == pRenderer)
		return E_FAIL;

	if (eGroup == RENDER_ALPHABLEND)
	{
		m_AlphaRenderers.push_back(make_pair(0.f, static_cast<CMeshRenderer*>(pRenderer)));
		return S_OK;
	}

	m_Renderers[eGroup].push_back(pRenderer);

	return S_OK;
}

HRESULT CRender_Manager::Render()
{
	if (FAILED(CShader_Manager::Get_Instance()->Begin_Shader()))
	{
		MSG_BOX("Failed to Begin_Shader : CRender_Manager");
		return E_FAIL;
	}

	//// ====================== Shadow Map =======================
	//DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (FAILED(CShader_Manager::Get_Instance()->Begin_ShadowMap()))
		return E_FAIL;

	for (auto& elem : m_Renderers[RENDER_NONALPHABLEND])
	{
		CMeshRenderer* pRenderer = static_cast<CMeshRenderer*>(elem);
		CShader_Manager::Get_Instance()->Set_WorldMatrix(pRenderer->Get_Owner()->Get_Transform()->Get_WorldMatrix());
		CShader_Manager::Get_Instance()->Begin_Pass(PASS_SHADOWMAP, pRenderer);

		if (FAILED(elem->Render()))
			return E_FAIL;

		CShader_Manager::Get_Instance()->End_Pass(PASS_SHADOWMAP);
	}

	if (FAILED(CShader_Manager::Get_Instance()->End_ShadowMap()))
		return E_FAIL;

	//// =========================================================



	/* ================== Render ==================== */

	for (_uint i = RENDER_PRIORITY; i < RENDER_PARTICLE; ++i)
	{
		for (auto& elem : m_Renderers[i])
		{
			CMeshRenderer* pRenderer = static_cast<CMeshRenderer*>(elem);

			PASS_TYPE	eType = pRenderer->Get_PassType();
			_float4x4 matWorld = pRenderer->Get_Owner()->Get_Transform()->Get_WorldMatrix();
			_float3 vOffsetPos = pRenderer->Get_OffsetPos();
			*(_float3*)(&matWorld.m[WORLD_POS]) += vOffsetPos;
			CShader_Manager::Get_Instance()->Set_WorldMatrix(matWorld);
			CShader_Manager::Get_Instance()->Begin_Pass(eType, pRenderer);

			if (FAILED(pRenderer->Render()))
				return E_FAIL;

			CShader_Manager::Get_Instance()->End_Pass(eType);
		}

		m_Renderers[i].clear();
	}

	/* ===============================================*/

	if (FAILED(CShader_Manager::Get_Instance()->End_Shader()))
	{
		MSG_BOX("Failed to End_Shader : CRender_Manager");
		return E_FAIL;
	}

	/* ============================= PARTICLE =========================== */
	if (FAILED(CShader_Manager::Get_Instance()->Begin_Particle_Shader()))
	{
		MSG_BOX("Failed to Begin_Particle_Shader : CRender_Manager");
		return E_FAIL;
	}

	DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	if (FAILED(Render_Group(RENDER_PARTICLE)))
		return E_FAIL;

	if (FAILED(CShader_Manager::Get_Instance()->End_Particle_Shader()))
	{
		MSG_BOX("Failed to End_Particle_Shader : CRender_Manager");
		return E_FAIL;
	}


	/* =========== Alpha Blending Group =========== */

	if (FAILED(CShader_Manager::Get_Instance()->Begin_Shader()))
	{
		MSG_BOX("Failed to Begin_Shader : CRender_Manager");
		return E_FAIL;
	}

	Sort_AlphaList();
	

	for (auto& elem : m_AlphaRenderers)
	{

		CMeshRenderer* pRenderer = elem.second;

		PASS_TYPE	eType = pRenderer->Get_PassType();
		_float4x4 matWorld = pRenderer->Get_Owner()->Get_Transform()->Get_WorldMatrix();
		_float3 vOffsetPos = pRenderer->Get_OffsetPos();
		*(_float3*)(&matWorld.m[WORLD_POS]) += vOffsetPos;
		CShader_Manager::Get_Instance()->Set_WorldMatrix(matWorld);
		CShader_Manager::Get_Instance()->Begin_Pass(eType, pRenderer);

		if (FAILED(pRenderer->Render()))
			return E_FAIL;

		CShader_Manager::Get_Instance()->End_Pass(eType);
	}

	m_AlphaRenderers.clear();

	if (FAILED(CLevel_Manager::Get_Instance()->Render()))
		return E_FAIL;

	

	/* ============================================ */


	/* ========================= ORTHO UI =============================*/

	if (FAILED(CShader_Manager::Get_Instance()->Set_float4x4("matView", CCamera_Manager::Get_Instance()->Get_OrthoView())))
		return E_FAIL;
	if (FAILED(CShader_Manager::Get_Instance()->Set_float4x4("matProj", CCamera_Manager::Get_Instance()->Get_OrthoProj())))
		return E_FAIL;

	for (auto& elem : m_Renderers[RENDER_ORTHO])
	{

		CMeshRenderer* pRenderer = static_cast<CMeshRenderer*>(elem);

		PASS_TYPE	eType = pRenderer->Get_PassType();
		_float4x4 matWorld = pRenderer->Get_Owner()->Get_Transform()->Get_WorldMatrix();
		_float3 vOffsetPos = pRenderer->Get_OffsetPos();
		*(_float3*)(&matWorld.m[WORLD_POS]) += vOffsetPos;
		CShader_Manager::Get_Instance()->Set_WorldMatrix(matWorld);

		CShader_Manager::Get_Instance()->Begin_Pass(eType, pRenderer);

		if (FAILED(pRenderer->Render()))
			return E_FAIL;

		CShader_Manager::Get_Instance()->End_Pass(eType);
	}

	m_Renderers[RENDER_ORTHO].clear();


	DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	if (FAILED(CShader_Manager::Get_Instance()->End_Shader()))
	{
		MSG_BOX("Failed to End_Shader : CRender_Manager");
		return E_FAIL;
	}
	/* ============================ UI RENDER =============================== */
	CGraphic_Device::Get_Instance()->Sprite_Render_Begin();

	if (FAILED(Render_Group(RENDER_UI)))
		return E_FAIL;

	/* ====================================================================== */

	return S_OK;
}

HRESULT CRender_Manager::Render_Group(RENDERGROUP eGroup)
{
	for (auto& elem : m_Renderers[eGroup])
		if (FAILED(elem->Render()))
			return E_FAIL;

	m_Renderers[eGroup].clear();

	return S_OK;
}

void CRender_Manager::Sort_AlphaList()
{
	_float3 vCamPos = CCamera_Manager::Get_Instance()->Get_ViewPos();

	for (auto& elem : m_AlphaRenderers)
	{
		_float3 vMyPos = elem.second->Get_Owner()->Get_Transform()->Get_MyWorld(WORLD_POS);
		_float fDistance = (vMyPos - vCamPos).Length();
		elem.first = fDistance;
	}

	m_AlphaRenderers.sort(greater<pair<_float, CMeshRenderer*>>());
}