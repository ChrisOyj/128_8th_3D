#include "..\Public\Picking_Manager.h"

#include "GameInstance.h"
#include "Transform.h"
#include "Camera_Manager.h"
#include "GameObject.h"

#include "Renderer.h"

CPicking_Manager::CPicking_Manager()
{
}


CPicking_Manager::~CPicking_Manager()
{
}

void CPicking_Manager::Add_Renderer(CRenderer* pRenderer, _float fDistanceToPlayer)
{
	m_PickingRenderers.push_back(make_pair(fDistanceToPlayer, pRenderer));
}

void CPicking_Manager::Execute_Picking()
{
	Compute_WorldRay();
	Sort_Lists();

	Picking();

	m_PickingRenderers.clear();
}

HRESULT CPicking_Manager::Initialize(const GRAPHICDESC& tGraphicDesc)
{
	m_hWnd = tGraphicDesc.hWnd;

	m_GraphicDesc = tGraphicDesc;

	return S_OK;
}

void CPicking_Manager::Compute_WorldRay()
{
	POINT pt{};
	
	::GetCursorPos(&pt);
	::ScreenToClient(m_hWnd, &pt);
	
	_float2 fPT = { (_float)pt.x, (_float)pt.y };
	//_float2 fPT = { 1280 * 0.5f, 768 * 0.5f };

	_float4 vProjPos =
	{
		fPT.x / (m_GraphicDesc.iWinCX * 0.5f) - 1.f,
		fPT.y / (m_GraphicDesc.iWinCY * -0.5f) + 1.f,
		0.f
	};

	/* To View */
	_float4x4	ProjMatrixInv = CCamera_Manager::Get_Instance()->Get_Proj();
	ProjMatrixInv.Inverse();

	_float4 vViewPos = vProjPos * ProjMatrixInv;

	_float4 vRayDir = vViewPos.Normalize();
	_float4 vRayPos = { 0.f, 0.f, 0.f };

	/* To World */
	_float4x4	ViewMatrixInv = CCamera_Manager::Get_Instance()->Get_View();
	ViewMatrixInv.Inverse();

	m_vRayDir = vRayDir * ViewMatrixInv;
	m_vRayPos = vRayPos * ViewMatrixInv;
}

_bool CPicking_Manager::Is_3DPicked(CRenderer* pRenderer, _float4*	pOut, _float4* pOutNormal)
{

	_float4 vRendererPos = pRenderer->Get_FinalPos();

	_float4x4 WorldMatrix = pRenderer->Get_Owner()->Get_Transform()->Get_WorldMatrix();

	*(_float4*)(&WorldMatrix.m[WORLD_POS]) = vRendererPos;

	WorldMatrix.Inverse();

	_float4			vRayPos = m_vRayPos;
	_float4			vRayDir = m_vRayDir;
	vRayPos *= (WorldMatrix);
	vRayDir *= (WorldMatrix, true);
	vRayDir.Normalize();



	/*CMesh*		pMesh = pRenderer->Get_Mesh();
	

	_uint			iNumFaces = pMesh->Get_NumFaces();
	const _float4*	pVerticesPos = pMesh->Get_VerticesPos();

	_float		fU, fV, fDist, fMin = 9999.f;
	WorldMatrix.Inverse();
	for (_uint i = 0; i < iNumFaces; ++i)
	{
		_uint3		iIndices = pMesh->Get_Indices(i);

		_float4		vPickedPos;

		_float4 pVertice1 = *(_float4*)(&pVerticesPos[iIndices._1]);
		_float4 pVertice2 = *(_float4*)(&pVerticesPos[iIndices._2]);
		_float4 pVertice3 = *(_float4*)(&pVerticesPos[iIndices._3]);

		if (XMPlaneIntersectLine(&pVertice1, &pVertice2, &pVertice3
			, &vRayPos, &vRayDir, &fU, &fV, &fDist))
		{
			vPickedPos = vRayPos + vRayDir * fDist;
			
			_float4 V1, V2;
			_float4 vOutNormal;
			_float4x4 worldMat = WorldMatrix;

			V1 = (pVertice1 - pVertice2);
			V2 = (pVertice3 - pVertice2);

			D3DXVec3Cross(&vOutNormal, &V2, &V1);

			vOutNormal.Multiply(worldMat, true).Normalize();
			vPickedPos.Multiply(worldMat);

			_float fDistance = (m_vRayPos - vPickedPos).Length();

			if (fMin > fDistance)
			{
				*pOutNormal = vOutNormal;
				*pOut = vPickedPos;
				fMin = fDistance;
			}

		}
	}

	if (fMin != 9999.f)
	{
		return true;
	}*/

	return false;
}

_bool CPicking_Manager::Is_OnMouseRay(CRenderer * pRenderer)
{
	return false;
}


void CPicking_Manager::Sort_Lists()
{
	m_PickingRenderers.sort();
}

void CPicking_Manager::Picking()
{
	_float4 vPickedPos = { 0.f,0.f,0.f };
	_float4 vPickedNormal = { 0.f,0.f,0.f };

	for (auto& ObjPair : m_PickingRenderers)
	{
		CRenderer* pRenderer = ObjPair.second;

		if (ObjPair.first == 999.f)
			int i = 0;

		if (Is_3DPicked(pRenderer, &vPickedPos, &vPickedNormal))
		{
			pRenderer->Get_Owner()->CallBack_PickingEvent(vPickedPos, vPickedNormal);
			break;
		}
	}

}