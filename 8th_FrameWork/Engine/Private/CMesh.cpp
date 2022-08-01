#include "CMesh.h"

#include "GameInstance.h"

CMesh::CMesh(_uint iGroupIdx)
    : CComponent(iGroupIdx)
{
}

CMesh::~CMesh()
{
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
}

HRESULT CMesh::Create_VertexBuffer()
{
    return (DEVICE->CreateBuffer(&m_BufferDesc, &m_SubResourceData, m_pVB.GetAddressOf()));
}

HRESULT CMesh::Create_IndexBuffer()
{
    return (DEVICE->CreateBuffer(&m_BufferDesc, &m_SubResourceData, m_pIB.GetAddressOf()));
}
