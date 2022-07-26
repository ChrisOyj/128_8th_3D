#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CGameObject;
class CRenderer;

class CPicking_Manager
{
	DECLARE_SINGLETON(CPicking_Manager)

private:
	CPicking_Manager();
	~CPicking_Manager();

public:
	HRESULT	Initialize(const GRAPHICDESC&	tGraphicDesc);

public:
	void	Add_Renderer(CRenderer* pRenderer, _float fDistanceToPlayer);
	void	Execute_Picking();

public:
	_bool	Is_3DPicked(CRenderer* pRenderer, _float4*	pOut, _float4* pOutNormal = nullptr);
	_bool	Is_OnMouseRay(CRenderer* pRenderer);

private:
	HWND	m_hWnd;
	list<pair<_float, CRenderer*>>	m_PickingRenderers;

	GRAPHICDESC		m_GraphicDesc;

	_float4			m_vRayDir;
	_float4			m_vRayPos;

private:
	void	Sort_Lists();
	void	Compute_WorldRay();
	void	Picking();
};

END