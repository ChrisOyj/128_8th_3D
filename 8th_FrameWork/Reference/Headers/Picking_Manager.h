#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CGameObject;
class CMesh;

class CPicking_Manager
{
	DECLARE_SINGLETON(CPicking_Manager)

private:
	CPicking_Manager();
	~CPicking_Manager();

public:
	HRESULT	Initialize(const GRAPHICDESC&	tGraphicDesc);

public:
	void	Regist_Mesh(CMesh* pMesh, _float fDistanceToPlayer);
	void	Execute_Picking();
	void	Compute_WorldRay();

public:
	_bool	Is_Picked(CMesh* pRenderer, _float4*	pOut, _float4* pOutNormal = nullptr);

private:
	list<pair<_float, CMesh*>>	m_PickingMeshes;
	GRAPHICDESC		m_GraphicDesc;

	_float3			m_vRayDir;
	_float3			m_vRayPos;

private:
	void	Sort_Lists();
	void	Picking();
};

END