#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CRenderTarget
{
private:
	CRenderTarget();
	virtual ~CRenderTarget();
	
	friend class CTarget_Manager;

public:
	static CRenderTarget* Create(_uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor);

public:
	ComPtr<ID3D11ShaderResourceView> Get_SRV() const {
		return m_pSRV;
	}

	ComPtr<ID3D11RenderTargetView> Get_RTV() const {
		return m_pRTV;
	}

public:
	/* 1. ID3D11Texture2D 라는 객체를 선 생성. */
	/* 2. ID3D11RenderTargetView를 생성. */
	/* 3. ID3D11ShaderResourceView를 생성. */
	HRESULT Initialize(_uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor);
	HRESULT Clear();
	void	Release();

#ifdef _DEBUG
	HRESULT Ready_Debug(_float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug(class CShader* pShader, class CMesh_Rect* pVIBuffer);
	HRESULT Render_Shadow_Debug(class CShader* pShader, class CMesh_Rect* pVIBuffer, _float4x4 worldMat);
#endif // _DEBUG

private:

	_float4					m_vClearColor;

#ifdef _DEBUG
private:
	_float4x4				m_WorldMatrix;

#endif // _DEBUG

private:
	ComPtr<ID3D11Texture2D> m_pTexture = nullptr;
	/* 렌더타겟으로 바인딩 할 수 있는 객체 .*/
	ComPtr<ID3D11RenderTargetView> m_pRTV = nullptr;

	/* 셰이더 전역변수로 바인딩 될 수 있는 객체. */
	ComPtr<ID3D11ShaderResourceView> m_pSRV = nullptr;


};

END