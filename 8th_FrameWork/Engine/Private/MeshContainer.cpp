#include "..\Public\MeshContainer.h"
#include "HIerarchyNode.h"
#include "Model.h"
#include "MeshContainer.h"

#include "CShader.h"

#include "GameObject.h"

#include "CResource_Mesh.h"

CMeshContainer::CMeshContainer(_uint iGroupIndex)
	: CMesh(iGroupIndex)
{
}

CMeshContainer::~CMeshContainer()
{
	Release();
}

CMeshContainer* CMeshContainer::Create(CResource_Mesh* pMeshResource, _float4x4 TransformMatrix)
{
	CMeshContainer* pInstance = new CMeshContainer(0);

	if (FAILED(pInstance->SetUp_MeshContainer(pMeshResource, TransformMatrix)))
	{
		Call_MsgBox(L"Failed to SetUp_MeshContainer : CMeshContainer");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CMeshContainer");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

_float4 CMeshContainer::Get_LocalPos()
{
	return _float4();
}

HRESULT CMeshContainer::Initialize_Prototype()
{
	m_OffsetMatrix.Identity();
	return S_OK;
}

HRESULT CMeshContainer::Initialize()
{
	return S_OK;
}

void CMeshContainer::Release()
{
	m_Bones.clear();
}

HRESULT CMeshContainer::SetUp_Ptr(CModel* pModel)
{
	// 1. 머테리얼 인덱슷 찾기
	m_iMaterialIndex = pModel->Find_MaterialIndex(m_MaterialName.c_str());
	m_vColor = pModel->Get_Materials()[m_iMaterialIndex].second.vColor;

	// 2. 하이라키 노드 연결
	m_Bones.clear();

	if (0 == m_iNumBones || 1 == m_iNumBones)
	{
		m_iNumBones = 1;
		CHierarchyNode* pHierarchyNode = nullptr;
		if (m_strRefBoneName.empty())
			return S_OK;

		pHierarchyNode = pModel->Find_HierarchyNode(m_strRefBoneName.c_str());

		if (nullptr == pHierarchyNode)
			return E_FAIL;

		m_Bones.push_back(pHierarchyNode);

		return S_OK;
	}

	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		CHierarchyNode* pHierarchyNode = pModel->Find_HierarchyNode(m_pBoneDatas[i].szName);
		if (!pHierarchyNode)
			return E_FAIL;

		_float4x4 matOffset = m_pBoneDatas[i].OffsetMatrix;
		pHierarchyNode->Set_OffsetMatrix(matOffset.Transpose());

		m_Bones.push_back(pHierarchyNode);
	}

	

	return S_OK;
}


void CMeshContainer::SetUp_BoneMatrices(class CShader* pShader, const char* pConstantName)
{
	if (m_iNumBones > 256)
		return;

	_uint		iIndex = 0;
	_float4x4			BoneMatrices[256];

	if (m_Bones.size() == 1)
	{
		for (auto& pBone : m_Bones)
		{
			XMStoreFloat4x4(&BoneMatrices[iIndex++], XMMatrixTranspose(pBone->Get_CombinedMatrix() * m_TransformMatrix.XMLoad()));
		}
	}
	else
	{
		for (auto& pBone : m_Bones)
		{
			XMStoreFloat4x4(&BoneMatrices[iIndex++], XMMatrixTranspose(pBone->Get_OffsetMatrix() * pBone->Get_CombinedMatrix() * m_TransformMatrix.XMLoad()));
		}
	}

	
	
	pShader->Set_RawValue(pConstantName, BoneMatrices, sizeof(_float4x4) * 256);

}

void CMeshContainer::Set_ShaderResourceFlag(CShader* pShader, const char* pConstantName)
{
	pShader->Set_RawValue(pConstantName, &m_vFlag, sizeof(_float4));
	pShader->Set_RawValue("g_fOutlinePower", &m_fOutline, sizeof(_float));

}

void CMeshContainer::Set_ShaderResource(CShader* pShader, const char* pConstantName)
{
	pShader->Set_RawValue(pConstantName, &m_vColor, sizeof(_float4));
}

HRESULT CMeshContainer::SetUp_MeshContainer(CResource_Mesh* pResource, _float4x4 TransformMatrix)
{
	strcpy_s(m_szName, pResource->Get_Name().c_str());

	MESH_DESC& tMeshDesc = pResource->Get_MeshDesc();
	
	m_iNumVertices = tMeshDesc.iNumVertices;
	m_iMaterialIndex = tMeshDesc.iMaterialIndex;
	m_TransformMatrix = TransformMatrix;
	m_iNumVertexBuffers = 1;
	m_vCenterPos = tMeshDesc.vCenterPos.MultiplyCoord(TransformMatrix);
	_float fRatio =  ((_float4*)&TransformMatrix.m[0])->Length();
	m_fMaxRange = tMeshDesc.fMaxRange * fRatio;

	
	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_pVerticesPos = new _float3[m_iNumVertices];

	VTXMODEL* pVertices = nullptr;
	VTXANIM* pAnimVertices = nullptr;

	if (tMeshDesc.eModelType == TYPE_NONANIM)
	{

		m_iStride = sizeof(VTXMODEL);
		pVertices = new VTXMODEL[m_iNumVertices];
		memcpy(pVertices, tMeshDesc.pVertices, sizeof(VTXMODEL) * m_iNumVertices);

		for (_uint i = 0; i < m_iNumVertices; ++i)
		{
			XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), TransformMatrix.XMLoad()));
			m_pVerticesPos[i] = pVertices[i].vPosition;

		}

		m_SubResourceData.pSysMem = pVertices;
	}
	else
	{
		for (_uint i = 0; i < m_iNumVertices; ++i)
			m_pVerticesPos[i] = tMeshDesc.pAnimVertices[i].vPosition;

		//pAnimVertices = new VTXANIM[m_iNumVertices];
		//memcpy(pAnimVertices, tMeshDesc.pAnimVertices, sizeof(VTXANIM) * m_iNumVertices);


		m_iStride = sizeof(VTXANIM);
		m_iNumBones = tMeshDesc.iNumBones;
		m_pBoneDatas = tMeshDesc.pBoneDatas;
		m_SubResourceData.pSysMem = tMeshDesc.pAnimVertices;
	}

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	HRESULT hr = __super::Create_VertexBuffer();

	if (FAILED(hr))
		return E_FAIL;

	SAFE_DELETE_ARRAY(pVertices);
	SAFE_DELETE_ARRAY(pAnimVertices);


	/* Index */

	m_iIndicesStride = sizeof(FACEINDICES32);
	m_iNumPrimitive = tMeshDesc.iNumFaces;
	m_iNumIndices = 3 * m_iNumPrimitive;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eToplogy = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iIndicesStride * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = 0;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	m_pIndices = new FACEINDICES32[m_iNumPrimitive];
	ZeroMemory(m_pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);
	memcpy(m_pIndices, tMeshDesc.pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);
	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	m_SubResourceData.pSysMem = tMeshDesc.pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	return S_OK;
}