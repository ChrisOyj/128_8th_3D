#include "CMesh.h"

#include "GameInstance.h"

CMesh::CMesh(_uint iGroupIdx)
    : CComponent(iGroupIdx)
{
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	ZeroMemory(&m_eIndexFormat, sizeof(DXGI_FORMAT));
	m_eToplogy = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
}

//CMesh::CMesh(const CMesh& origin)
//	: CComponent(origin)
//	, m_BufferDesc(origin.m_BufferDesc)
//	, m_SubResourceData(origin.m_SubResourceData)
//	, m_eIndexFormat(origin.m_eIndexFormat)
//	, m_eToplogy(origin.m_eToplogy)
//	, m_iIndicesStride(origin.m_iIndicesStride)
//	, m_iMeshType(origin.m_iMeshType)
//	, m_iNumIndices(origin.m_iNumIndices)
//{
//}

CMesh::~CMesh()
{
	Release();
}

void CMesh::Tick()
{
}

void CMesh::Late_Tick()
{
}

HRESULT CMesh::Render()
{
	ID3D11Buffer* pVertexBuffers[] = {
		m_pVB.Get(),
	};

	_uint		iStrides[] = {
		m_iStride,
	};

	_uint		iOffsets[] = {
		0,
	};

	DEVICE_CONTEXT->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iStrides, iOffsets);
	DEVICE_CONTEXT->IASetIndexBuffer(m_pIB.Get(), m_eIndexFormat, 0);
	DEVICE_CONTEXT->IASetPrimitiveTopology(m_eToplogy);

	DEVICE_CONTEXT->DrawIndexed(m_iNumIndices, 0, 0);

	return S_OK;
}

void CMesh::Release()
{
	_ulong dwCnt = m_pIB.Reset();
	dwCnt = m_pVB.Reset();

	if (dwCnt == 0)
	{
		SAFE_DELETE(m_pVerticesPos);
		SAFE_DELETE(m_pIndices);
	}
	
}

HRESULT CMesh::Create_VertexBuffer()
{
    return (DEVICE->CreateBuffer(&m_BufferDesc, &m_SubResourceData, m_pVB.GetAddressOf()));
}

HRESULT CMesh::Create_IndexBuffer()
{
    return (DEVICE->CreateBuffer(&m_BufferDesc, &m_SubResourceData, m_pIB.GetAddressOf()));
}
