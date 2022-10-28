#include "..\Public\Picking_Manager.h"

#include "GameInstance.h"
#include "Transform.h"
#include "Camera_Manager.h"
#include "GameObject.h"

#include "CMesh.h"

IMPLEMENT_SINGLETON(CPicking_Manager)


CPicking_Manager::CPicking_Manager()
{
}


CPicking_Manager::~CPicking_Manager()
{
}

void CPicking_Manager::Regist_Mesh(CMesh* pMesh, _float fDistanceToPlayer)
{
	m_PickingMeshes.push_back(make_pair(fDistanceToPlayer, pMesh));

}

void CPicking_Manager::Execute_Picking()
{
	if (m_PickingMeshes.empty())
		return;

	Compute_WorldRay();
	Sort_Lists();

	Picking();

	m_PickingMeshes.clear();
}

_bool CPicking_Manager::Is_Picked(CMesh* pMesh, _float4* pOut, _float4* pOutNormal)
{
	CTransform* pTransform = pMesh->Get_Owner()->Get_Transform();
	_float4x4	matWorld = pTransform->Get_WorldMatrix();
	_matrix		WorldMatrixInv = matWorld.Inverse().XMLoad();
	matWorld.Inverse();

	_vector			vRayPos, vRayDir;

	vRayPos = XMVector3TransformCoord(XMLoadFloat3(&m_vRayPos), WorldMatrixInv);
	vRayDir = XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_vRayDir), WorldMatrixInv));

	_uint			iNumFaces = pMesh->Get_NumPrimitive();
	const _float3* pVerticesPos = pMesh->Get_VerticesPos();
	_uint			iIndexSize = pMesh->Get_IndexSize();

	_float		fDist, fMin = 9999.f;

	for (_uint i = 0; i < iNumFaces; ++i)
	{
		_uint3		iIndices = pMesh->Get_Indices(i);

		_vector		vVec0 = XMLoadFloat3(&pVerticesPos[iIndices._1]);
		GXMVECTOR	vVec1 = XMLoadFloat3(&pVerticesPos[iIndices._2]);
		HXMVECTOR	vVec2 = XMLoadFloat3(&pVerticesPos[iIndices._3]);

		if (true == TriangleTests::Intersects(vRayPos, vRayDir, vVec0, vVec1, vVec2, fDist))
		{
			_float4 V1, V2;
			_float4 vOutNormal, vPickedPos;
			_float4x4 worldMat = matWorld;

			V1 = (vVec0 - vVec1);
			V2 = (vVec2 - vVec1);

			vOutNormal = XMVector3Cross(V1.XMLoad(), V2.XMLoad());
			vOutNormal = vOutNormal.MultiplyNormal(worldMat);
			vOutNormal.Normalize();
			vPickedPos = vRayPos + XMVector3Normalize(vRayDir) * fDist;

			_float4 vRayPos = _float4(m_vRayPos.x, m_vRayPos.y, m_vRayPos.z, 1.f);

			_float fDistance = (vRayPos - vPickedPos).Length();

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

HRESULT CPicking_Manager::Initialize(const GRAPHICDESC& tGraphicDesc)
{
	m_GraphicDesc = tGraphicDesc;

	return S_OK;
}

void CPicking_Manager::Compute_WorldRay()
{
	POINT pt{};
	
	::GetCursorPos(&pt);
	::ScreenToClient(m_GraphicDesc.hWnd, &pt);
	
	_float2 fPT = { (_float)pt.x, (_float)pt.y };

	_float4 vProjPos =
	{
		fPT.x / (m_GraphicDesc.iWinCX * 0.5f) - 1.f,
		fPT.y / -(m_GraphicDesc.iWinCY * 0.5f) + 1.f,
		0.f, 1.f
	};

	/* To View */
	_float4x4	ProjMatrixInv = CCamera_Manager::Get_Instance()->Get_Proj();
	ProjMatrixInv.Inverse();

	_float4 vViewPos = vProjPos.MultiplyCoord(ProjMatrixInv);
	
	_float4 vRayDir = vViewPos.Normalize();
	_float4 vRayPos = { 0.f, 0.f, 0.f, 1.f };

	/* To World */
	_float4x4	ViewMatrixInv = CCamera_Manager::Get_Instance()->Get_View();
	ViewMatrixInv.Inverse();

	vRayDir = vRayDir.MultiplyNormal(ViewMatrixInv);
	vRayPos = vRayPos.MultiplyCoord(ViewMatrixInv);

	vRayDir.Normalize();
	m_vRayDir = { vRayDir.x, vRayDir.y, vRayDir.z };
	m_vRayPos = { vRayPos.x, vRayPos.y, vRayPos.z };
}

void CPicking_Manager::Sort_Lists()
{
	m_PickingMeshes.sort();
}

void CPicking_Manager::Picking()
{
	_float4 vPickedPos = { 0.f,0.f,0.f };
	_float4 vPickedNormal = { 0.f,0.f,0.f };

	for (auto& ObjPair : m_PickingMeshes)
	{
		CMesh* pMesh = ObjPair.second;

		if (Is_Picked(pMesh, &vPickedPos, &vPickedNormal))
		{
			pMesh->Get_Owner()->CallBack_PickingEvent(vPickedPos, vPickedNormal);
			break;
		}
	}

}