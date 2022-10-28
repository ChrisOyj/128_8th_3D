#include "CRect_Instance.h"

#include "GameInstance.h"
#include "CShader.h"

#include "GameObject.h"

#include "CResource_Mesh.h"
CRect_Instance::CRect_Instance(_uint iGroupIndex)
	: CMesh_Rect(iGroupIndex)
{
}

CRect_Instance::CRect_Instance(const CRect_Instance& _origin)
	: CMesh_Rect(_origin)
	, m_iNumInstance(_origin.m_iNumInstance)
	, m_iInstanceStride(_origin.m_iInstanceStride)
{
	SetUp_InstanceMesh(m_iNumInstance);

}

CRect_Instance::~CRect_Instance()
{
	m_pVBInstance.Reset();
}

CRect_Instance* CRect_Instance::Create(_uint iNumInstance)
{
	CRect_Instance* pInstance = new CRect_Instance(0);
	
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CRect_Instance");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->SetUp_InstanceMesh(iNumInstance)))
	{
		Call_MsgBox(L"Failed to SetUp_MeshContainer : CRect_Instance");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CRect_Instance::ReMake_Instance(_uint iNumInstance)
{
	m_pVBInstance.Reset();

	SetUp_InstanceMesh(iNumInstance);
}

void CRect_Instance::ReMap_Instances(VTXRECTINSTANCE* pRectInstances)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource;

	DEVICE_CONTEXT->Map(m_pVBInstance.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	memcpy(SubResource.pData, pRectInstances, sizeof(VTXRECTINSTANCE) * m_iNumInstance);


	DEVICE_CONTEXT->Unmap(m_pVBInstance.Get(), 0);


}

void CRect_Instance::Release()
{
}

HRESULT CRect_Instance::Render()
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

HRESULT CRect_Instance::SetUp_InstanceMesh(_uint iNumInstance)
{
	m_iNumVertexBuffers = 2;

	m_iInstanceStride = sizeof(VTXRECTINSTANCE);
	m_iNumInstance = iNumInstance;


	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iInstanceStride * m_iNumInstance;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iInstanceStride;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;

	VTXRECTINSTANCE* pInstance = new VTXRECTINSTANCE[m_iNumInstance];
	ZeroMemory(pInstance, sizeof(VTXRECTINSTANCE) * m_iNumInstance);


	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pInstance;

	if (FAILED(DEVICE->CreateBuffer(&m_BufferDesc, &m_SubResourceData, m_pVBInstance.GetAddressOf())))
		return E_FAIL;

	Safe_Delete_Array(pInstance);

	return S_OK;
}
