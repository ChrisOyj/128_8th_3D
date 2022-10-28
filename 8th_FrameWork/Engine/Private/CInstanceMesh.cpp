#include "CInstanceMesh.h"

#include "GameInstance.h"
#include "CShader.h"

#include "GameObject.h"

#include "CResource_Mesh.h"
CInstanceMesh::CInstanceMesh(_uint iGroupIndex)
	: CMeshContainer(iGroupIndex)
{
}

CInstanceMesh::CInstanceMesh(const CInstanceMesh& _origin)
	: CMeshContainer(_origin)
	, m_iNumInstance(_origin.m_iNumInstance)
	, m_iInstanceStride(_origin.m_iInstanceStride)
{
	SetUp_InstanceMesh(m_iNumInstance);

}

CInstanceMesh::~CInstanceMesh()
{
	m_pVBInstance.Reset();
}

CInstanceMesh* CInstanceMesh::Create(wstring wstrInstanceDataPath, CResource_Mesh* pMeshResource, _float4x4 TransformMatrix)
{
	CInstanceMesh* pInstance = new CInstanceMesh(0);

	if (FAILED(pInstance->SetUp_MeshContainer(pMeshResource, TransformMatrix)))
	{
		Call_MsgBox(L"Failed to SetUp_MeshContainer : CInstanceMesh");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->SetUp_InstanceMesh(wstrInstanceDataPath, pMeshResource, TransformMatrix)))
	{
		Call_MsgBox(L"Failed to SetUp_MeshContainer : CInstanceMesh");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CInstanceMesh");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

CInstanceMesh* CInstanceMesh::Create(_uint iNumInstance, CResource_Mesh* pMeshResource, _float4x4 TransformMatrix)
{
	CInstanceMesh* pInstance = new CInstanceMesh(0);

	if (FAILED(pInstance->SetUp_MeshContainer(pMeshResource, TransformMatrix)))
	{
		Call_MsgBox(L"Failed to SetUp_MeshContainer : CInstanceMesh");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->SetUp_InstanceMesh(iNumInstance)))
	{
		Call_MsgBox(L"Failed to SetUp_MeshContainer : CInstanceMesh");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CInstanceMesh");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CInstanceMesh::ReMap_Instances(_float4x4* pInstancesMatrices)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource;

	DEVICE_CONTEXT->Map(m_pVBInstance.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	memcpy(SubResource.pData, pInstancesMatrices, sizeof(VTXINSTANCE) * m_iNumInstance);

	/*for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		((VTXINSTANCE*)SubResource.pData)[i].vRight.x = pInstancesMatrices[i].m[0][0];
		((VTXINSTANCE*)SubResource.pData)[i].vRight.y = pInstancesMatrices[i].m[0][1];
		((VTXINSTANCE*)SubResource.pData)[i].vRight.z = pInstancesMatrices[i].m[0][2];
		((VTXINSTANCE*)SubResource.pData)[i].vRight.w = pInstancesMatrices[i].m[0][3];

		((VTXINSTANCE*)SubResource.pData)[i].vUp.x = pInstancesMatrices[i].m[1][0];
		((VTXINSTANCE*)SubResource.pData)[i].vUp.y = pInstancesMatrices[i].m[1][1];
		((VTXINSTANCE*)SubResource.pData)[i].vUp.z = pInstancesMatrices[i].m[1][2];
		((VTXINSTANCE*)SubResource.pData)[i].vUp.w = pInstancesMatrices[i].m[1][3];

		((VTXINSTANCE*)SubResource.pData)[i].vLook.x = pInstancesMatrices[i].m[2][0];
		((VTXINSTANCE*)SubResource.pData)[i].vLook.y = pInstancesMatrices[i].m[2][1];
		((VTXINSTANCE*)SubResource.pData)[i].vLook.z = pInstancesMatrices[i].m[2][2];
		((VTXINSTANCE*)SubResource.pData)[i].vLook.w = pInstancesMatrices[i].m[2][3];

		((VTXINSTANCE*)SubResource.pData)[i].vTranslation.x = pInstancesMatrices[i].m[3][0];
		((VTXINSTANCE*)SubResource.pData)[i].vTranslation.y = pInstancesMatrices[i].m[3][1];
		((VTXINSTANCE*)SubResource.pData)[i].vTranslation.z = pInstancesMatrices[i].m[3][2];
		((VTXINSTANCE*)SubResource.pData)[i].vTranslation.w = pInstancesMatrices[i].m[3][3];

	}*/


	DEVICE_CONTEXT->Unmap(m_pVBInstance.Get(), 0);


}

void CInstanceMesh::Release()
{
}

HRESULT CInstanceMesh::Render()
{
	ID3D11Buffer* pVertexBuffers[] = {
		m_pVB.Get(),
		m_pVBInstance.Get()
	};

	_uint		iStrides[] = {
		m_iStride,
		m_iInstanceStride
	};

	_uint		iOffsets[] = {
		0,
		0
	};

	DEVICE_CONTEXT->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iStrides, iOffsets);
	DEVICE_CONTEXT->IASetIndexBuffer(m_pIB.Get(), m_eIndexFormat, 0);
	DEVICE_CONTEXT->IASetPrimitiveTopology(m_eToplogy);

	/* 6 : 하나의 도형을 그리기위해 사용하는 인덱스의 갯수. 네모라서 여섯개.  */
	DEVICE_CONTEXT->DrawIndexedInstanced(m_iNumIndices, m_iNumInstance, 0, 0, 0);

	return S_OK;
}

HRESULT CInstanceMesh::SetUp_InstanceMesh(wstring wstrInstanceDataPath, CResource_Mesh* pResource, _float4x4 TransformMatrix)
{
	m_iNumVertexBuffers = 2;

	ifstream	readFile(wstrInstanceDataPath, ios::binary);

	if (!readFile.is_open())
	{
		return E_FAIL;
	}


	MESH_DESC& tMeshDesc = pResource->Get_MeshDesc();

	m_iInstanceStride = sizeof(VTXINSTANCE);
	readFile.read((char*)&m_iNumInstance, sizeof(_uint));


	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iInstanceStride * m_iNumInstance;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iInstanceStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXINSTANCE* pInstance = new VTXINSTANCE[m_iNumInstance];
	readFile.read((char*)pInstance, sizeof(VTXINSTANCE) * m_iNumInstance);


	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pInstance;

	if (FAILED(DEVICE->CreateBuffer(&m_BufferDesc, &m_SubResourceData, m_pVBInstance.GetAddressOf())))
		return E_FAIL;

	Safe_Delete_Array(pInstance);

	return S_OK;
}

HRESULT CInstanceMesh::SetUp_InstanceMesh(_uint iNumInstance)
{
	m_iNumVertexBuffers = 2;

	m_iInstanceStride = sizeof(VTXINSTANCE);
	m_iNumInstance = iNumInstance;


	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iInstanceStride * m_iNumInstance;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iInstanceStride;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;

	VTXINSTANCE* pInstance = new VTXINSTANCE[m_iNumInstance];
	ZeroMemory(pInstance, sizeof(VTXINSTANCE) * m_iNumInstance);


	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pInstance;

	if (FAILED(DEVICE->CreateBuffer(&m_BufferDesc, &m_SubResourceData, m_pVBInstance.GetAddressOf())))
		return E_FAIL;

	Safe_Delete_Array(pInstance);

	return S_OK;
}
