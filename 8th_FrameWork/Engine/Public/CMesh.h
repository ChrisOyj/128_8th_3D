#pragma once
#include "Component.h"

BEGIN(Engine)
class CMesh abstract
	: public CComponent
{
protected:
	CMesh(CGameObject* pOwner);
	CMesh(const CMesh& _origin);
	virtual ~CMesh();

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
	ID3D11Buffer* m_pVB = nullptr;
	ID3D11Buffer* m_pIB = nullptr;
	_uint							m_iStride = 0;
	_uint							m_iNumVertices = 0;
	_uint							m_iNumPrimitive = 0;
	_uint							m_iIndicesStride = 0;
	_uint							m_iNumIndices = 0;
	_uint							m_iNumVertexBuffers = 0;
	DXGI_FORMAT						m_eIndexFormat;
	D3D11_PRIMITIVE_TOPOLOGY		m_eToplogy;

};
END
