#include "..\Public\Picking_Manager.h"

#include "GameInstance.h"
#include "UI.h"
#include "Transform.h"
#include "MeshRenderer.h"

#include "Mesh_Cube.h"

CPicking_Manager::CPicking_Manager()
{
	ZeroMemory(&m_Viewport, sizeof(D3DVIEWPORT9));
}


CPicking_Manager::~CPicking_Manager()
{
}

void CPicking_Manager::Add_Object(CGameObject * pGameObject, _float fDistanceToPlayer)
{
	m_PickingObject.push_back(make_pair(fDistanceToPlayer, pGameObject));
}

void CPicking_Manager::Add_UI(CUI * pUI)
{
	m_PickingUI.push_back(pUI);
}

HRESULT CPicking_Manager::Initialize(HWND hWnd)
{
	m_hWnd = hWnd;
	if (FAILED(DEVICE->GetViewport(&m_Viewport)))
		return E_FAIL;

	return S_OK;
}

void CPicking_Manager::Compute_WorldRay()
{
	POINT pt{};
	if (m_bRayCenterAnytime)//이게 true일떄는 뷰포트 사이즈를 받아와서 절판 = 화면의 중앙
	{
		D3DVIEWPORT9 viewPort;
		DEVICE->GetViewport(&viewPort);
		pt.x = viewPort.Width / 2;
		pt.y = viewPort.Height/ 2;
	}
	else
	{
		::GetCursorPos(&pt);
		::ScreenToClient(m_hWnd, &pt);
	}
	_float2 fPT = { (_float)pt.x, (_float)pt.y };
	//_float2 fPT = { 1280 * 0.5f, 768 * 0.5f };

	_float3 vProjPos =
	{
		fPT.x / (m_Viewport.Width * 0.5f) - 1.f,
		fPT.y / (m_Viewport.Height * -0.5f) + 1.f,
		0.f
	};

	/* To View */
	_float4x4	ProjMatrixInv = CCamera_Manager::Get_Instance()->Get_Proj();
	ProjMatrixInv.Inverse();

	_float3 vViewPos = vProjPos.Multiply(ProjMatrixInv);

	_float3 vRayDir = vViewPos.Normalize();
	_float3 vRayPos = { 0.f, 0.f, 0.f };

	/* To World */
	_float4x4	ViewMatrixInv = CCamera_Manager::Get_Instance()->Get_View();
	ViewMatrixInv.Inverse();

	m_vRayDir = vRayDir.Multiply(ViewMatrixInv, true);
	m_vRayPos = vRayPos.Multiply(ViewMatrixInv);
}

void CPicking_Manager::Tick()
{
	Compute_WorldRay();
	Sort_Lists();

	if (m_bPicking)
		Picking();

	else if (m_bRandomPicking)
	{
		for (_int i = 0; i < 8; ++i)
		{
			RandomPicking(0.1f);
		}
	}

	m_PickingObject.clear();
	m_PickingUI.clear();

}


_bool CPicking_Manager::Is3DPicked(CGameObject* pGameObject, _float3*	pOut, _float3* pOutNormal)
{
	CMeshRenderer*	pRenderer = pGameObject->Get_Component<CMeshRenderer>();
	if (!pRenderer)
		return false;

	_float3 vOffset = pRenderer->Get_OffsetPos();
	_float4x4		WorldMatrix = pGameObject->Get_Transform()->Get_WorldMatrix();
	_float4x4		tempWorld = WorldMatrix;

	*(_float3*)(&tempWorld.m[WORLD_POS]) = _float3(0.f, 0.f, 0.f);
	vOffset.Multiply(tempWorld);

	*(_float3*)(&WorldMatrix.m[WORLD_POS]) += vOffset;

	WorldMatrix.Inverse();

	_float3			vRayPos = m_vRayPos;
	_float3			vRayDir = m_vRayDir;
	vRayPos.Multiply(WorldMatrix);
	vRayDir.Multiply(WorldMatrix, true);
	vRayDir.Normalize();



	CMesh*		pMesh = pRenderer->Get_Mesh();
	

	_uint			iNumFaces = pMesh->Get_NumFaces();
	const _float3*	pVerticesPos = pMesh->Get_VerticesPos();

	_float		fU, fV, fDist, fMin = 9999.f;
	WorldMatrix.Inverse();
	for (_uint i = 0; i < iNumFaces; ++i)
	{
		_uint3		iIndices = pMesh->Get_Indices(i);

		_float3		vPickedPos;

		_float3 pVertice1 = *(_float3*)(&pVerticesPos[iIndices._1]);
		_float3 pVertice2 = *(_float3*)(&pVerticesPos[iIndices._2]);
		_float3 pVertice3 = *(_float3*)(&pVerticesPos[iIndices._3]);

		if (D3DXIntersectTri(&pVertice1, &pVertice2, &pVertice3
			, &vRayPos, &vRayDir, &fU, &fV, &fDist))
		{
			vPickedPos = vRayPos + vRayDir * fDist;
			
			_float3 V1, V2;
			_float3 vOutNormal;
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
	}

	return false;
}

_bool CPicking_Manager::IsOnMouseRay(CGameObject * pGameObject)
{
	CMeshRenderer*	pRenderer = pGameObject->Get_Component<CMeshRenderer>();
	if (!pRenderer)
		return false;

	_float3 vOffset = pRenderer->Get_OffsetPos();
	_float4x4		WorldMatrix = pGameObject->Get_Transform()->Get_WorldMatrix();
	_float4x4		tempWorld = WorldMatrix;

	*(_float3*)(&tempWorld.m[WORLD_POS]) = _float3(0.f, 0.f, 0.f);
	vOffset.Multiply(tempWorld);

	*(_float3*)(&WorldMatrix.m[WORLD_POS]) += vOffset;

	WorldMatrix.Inverse();

	_float3			vRayPos = m_vRayPos;
	_float3			vRayDir = m_vRayDir;
	vRayPos.Multiply(WorldMatrix);
	vRayDir.Multiply(WorldMatrix, true);
	vRayDir.Normalize();

	

	CMesh*		pMesh = pRenderer->Get_Mesh();


	_uint			iNumFaces = pMesh->Get_NumFaces();
	const _float3*	pVerticesPos = pMesh->Get_VerticesPos();

	_float		fU, fV, fDist, fMin = 9999.f;

	for (_uint i = 0; i < iNumFaces; ++i)
	{
		_uint3		iIndices = pMesh->Get_Indices(i);

		_float3		vPickedPos;

		_float3 pVertice1 = *(_float3*)(&pVerticesPos[iIndices._1]);
		_float3 pVertice2 = *(_float3*)(&pVerticesPos[iIndices._2]);
		_float3 pVertice3 = *(_float3*)(&pVerticesPos[iIndices._3]);

		if (D3DXIntersectTri(&pVertice1, &pVertice2, &pVertice3
			, &vRayPos, &vRayDir, &fU, &fV, &fDist))
		{
			return true;
		}
	}

	return false;
}

_bool CPicking_Manager::Is2DPicked(CUI * pGameObject)
{
	return _bool();
}


_bool CPicking_Manager::IsRandom3DPicked(CGameObject * pGameObject, _float3 * pOut, _float fRange, _float3 * pOutNormal)
{
	CMeshRenderer*	pRenderer = pGameObject->Get_Component<CMeshRenderer>();
	if (!pRenderer)
		return false;

	_float3 vOffset = pRenderer->Get_OffsetPos();
	_float4x4		WorldMatrix = pGameObject->Get_Transform()->Get_WorldMatrix();
	_float4x4		tempWorld = WorldMatrix;

	*(_float3*)(&tempWorld.m[WORLD_POS]) = _float3(0.f, 0.f, 0.f);
	vOffset.Multiply(tempWorld);

	*(_float3*)(&WorldMatrix.m[WORLD_POS]) += vOffset;

	WorldMatrix.Inverse();

	_float			fRandomX = frandom(-fRange, fRange);
	_float			fRandomY = frandom(-fRange, fRange);

	_float3			vRayPos = m_vRayPos;
	_float3			vRayDir = _float3(m_vRayDir.x + fRandomX, m_vRayDir.y + fRandomY, m_vRayDir.z);
	vRayPos.Multiply(WorldMatrix);
	vRayDir.Multiply(WorldMatrix, true);
	vRayDir.Normalize();

	CMesh*		pMesh = pRenderer->Get_Mesh();


	_uint			iNumFaces = pMesh->Get_NumFaces();
	const _float3*	pVerticesPos = pMesh->Get_VerticesPos();

	_float		fU, fV, fDist, fMin = 9999.f;
	WorldMatrix.Inverse();

	for (_uint i = 0; i < iNumFaces; ++i)
	{
		_uint3		iIndices = pMesh->Get_Indices(i);

		_float3		vPickedPos;

		_float3 pVertice1 = *(_float3*)(&pVerticesPos[iIndices._1]);
		_float3 pVertice2 = *(_float3*)(&pVerticesPos[iIndices._2]);
		_float3 pVertice3 = *(_float3*)(&pVerticesPos[iIndices._3]);

		if (D3DXIntersectTri(&pVertice1, &pVertice2, &pVertice3
			, &vRayPos, &vRayDir, &fU, &fV, &fDist))
		{
			vPickedPos = vRayPos + vRayDir * fDist;

			_float3 V1, V2;
			_float3 vOutNormal;
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
	}

	return false;
}


void CPicking_Manager::Sort_Lists()
{
	m_PickingObject.sort();
}

void CPicking_Manager::Picking()
{
	_float3 vPickedPos = { 0.f,0.f,0.f };
	_float3 vPickedNormal = { 0.f,0.f,0.f };

	for (auto& ObjPair : m_PickingObject)
	{
		CGameObject* pGameObject = ObjPair.second;

		if (ObjPair.first == 999.f)
			int i = 0;

		if (Is3DPicked(pGameObject, &vPickedPos, &vPickedNormal))
		{
			pGameObject->OnPickingEvent(vPickedPos, vPickedNormal);
			break;
		}
	}

	for (auto& pUI : m_PickingUI)
	{
		if (Is2DPicked(pUI))
		{
			pUI->OnPickingEvent(vPickedPos);
			break;
		}
	}

	m_bPicking = false;

}

void CPicking_Manager::RandomPicking( _float fRange)
{
	_float3 vPickedPos = { 0.f,0.f,0.f };
	_float3 vPickedNormal = { 0.f,0.f,0.f };

	for (auto& ObjPair : m_PickingObject)
	{
		CGameObject* pGameObject = ObjPair.second;
			if (IsRandom3DPicked(pGameObject, &vPickedPos, fRange, &vPickedNormal))
			{
				pGameObject->OnPickingEvent(vPickedPos, vPickedNormal);
				break;
			}
	}

	m_bRandomPicking = false;

}
