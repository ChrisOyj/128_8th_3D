#include "CTri_Instance.h"

#include "GameInstance.h"
#include "CShader.h"

#include "GameObject.h"

#include "CResource_Mesh.h"
CTri_Instance::CTri_Instance(_uint iGroupIndex)
	: CMesh(iGroupIndex)
{
}

CTri_Instance::CTri_Instance(const CTri_Instance& _origin)
	: CMesh(_origin)
	, m_iNumInstance(_origin.m_iNumInstance)
	, m_iInstanceStride(_origin.m_iInstanceStride)
{
	SetUp_InstanceMesh(m_iNumInstance);
}

CTri_Instance::~CTri_Instance()
{
}

CTri_Instance* CTri_Instance::Create(_uint iNumInstance)
{
	CTri_Instance* pInstance = new CTri_Instance(0);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CTri_Instance");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->SetUp_InstanceMesh(iNumInstance)))
	{
		Call_MsgBox(L"Failed to SetUp_MeshContainer : CTri_Instance");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTri_Instance::ReMap_Instances(VTXTRIINSTANCE* pTriInstances)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource;

	DEVICE_CONTEXT->Map(m_pVBInstance.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	memcpy(SubResource.pData, pTriInstances, sizeof(VTXTRIINSTANCE) * m_iNumInstance);


	DEVICE_CONTEXT->Unmap(m_pVBInstance.Get(), 0);


}

HRESULT CTri_Instance::Initialize_Prototype()
{
#pragma region VERTEXBUFFER
	m_iStride = sizeof(VTXDEFAULT);
	m_iNumVertices = 3;
	m_iNumVertexBuffers = 2;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXDEFAULT* pVertices = new VTXDEFAULT[m_iNumVertices];

	pVertices[0].vPosition = _float3(1.f, 0.f, 0.f);

	pVertices[1].vPosition = _float3(0.0f, 1.f, 0.f);

	pVertices[2].vPosition = _float3(0.f, 0.f, 1.f);

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
#pragma endregion

#pragma region INDEXBUFFER


#pragma endregion
	m_iIndicesStride = sizeof(FACEINDICES16);
	m_iNumPrimitive = 1;
	m_iNumIndices = 3 * m_iNumPrimitive;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_eToplogy = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iIndicesStride * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = 0;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	FACEINDICES16* pIndices = new FACEINDICES16[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES16) * m_iNumPrimitive);

	pIndices[0]._1 = 0;
	pIndices[0]._2 = 1;
	pIndices[0]._3 = 2;

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);


	return S_OK;
}

HRESULT CTri_Instance::Initialize()
{
	return S_OK;
}

HRESULT CTri_Instance::Render()
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

HRESULT CTri_Instance::SetUp_InstanceMesh(_uint iNumInstance)
{
	m_iNumVertexBuffers = 2;

	m_iInstanceStride = sizeof(VTXTRIINSTANCE);
	m_iNumInstance = iNumInstance;


	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iInstanceStride * m_iNumInstance;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iInstanceStride;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;

	VTXTRI_INSTANCE_DECLARATION* pInstance = new VTXTRI_INSTANCE_DECLARATION[m_iNumInstance];
	ZeroMemory(pInstance, sizeof(VTXTRI_INSTANCE_DECLARATION) * m_iNumInstance);


	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pInstance;

	if (FAILED(DEVICE->CreateBuffer(&m_BufferDesc, &m_SubResourceData, m_pVBInstance.GetAddressOf())))
		return E_FAIL;

	Safe_Delete_Array(pInstance);

	return S_OK;
}
