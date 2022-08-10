#pragma once
#include "Component.h"

BEGIN(Engine)
class ENGINE_DLL CMesh abstract
	: public CComponent
{
protected:
	CMesh(_uint iGroupIdx);
	virtual ~CMesh();

public:
	_uint		Get_MeshType() { return m_iMeshType; }
	void		Set_MeshType(_uint iMeshType) { m_iMeshType = iMeshType; }

public:
	// CComponent을(를) 통해 상속됨
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual HRESULT Render();
	virtual void Release() override;

	/* Non_Update_Component */
	virtual void Start() override {}
	virtual void OnEnable() override {}
	virtual void OnDisable() override {}


protected:
	D3D11_BUFFER_DESC				m_BufferDesc;
	D3D11_SUBRESOURCE_DATA			m_SubResourceData;

protected:
	ComPtr<ID3D11Buffer> m_pVB = nullptr;
	ComPtr<ID3D11Buffer> m_pIB = nullptr;
	_uint							m_iStride = 0;
	_uint							m_iNumVertices = 0;
	_uint							m_iNumPrimitive = 0;
	_uint							m_iIndicesStride = 0;
	_uint							m_iNumIndices = 0;
	_uint							m_iNumVertexBuffers = 0;
	DXGI_FORMAT						m_eIndexFormat;
	D3D11_PRIMITIVE_TOPOLOGY		m_eToplogy;
	_uint							m_iMeshType = 0;

protected:
	HRESULT Create_VertexBuffer();
	HRESULT Create_IndexBuffer();

};
END
