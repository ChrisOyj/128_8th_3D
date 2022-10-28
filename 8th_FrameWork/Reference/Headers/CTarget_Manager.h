#pragma once

/* 렌더타겟들을 모아놓는다. */
#include "Engine_Defines.h"

BEGIN(Engine)

class CTarget_Manager
{
	DECLARE_SINGLETON(CTarget_Manager)

private:
	CTarget_Manager();
	virtual ~CTarget_Manager();

public:
	ComPtr<ID3D11ShaderResourceView> Get_SRV(wstring pTargetTag);

	void	Set_ShadowDSV(ComPtr<ID3D11DepthStencilView> pDSV, D3D11_VIEWPORT shadowViewPort);

public:
	HRESULT Add_RenderTarget(wstring pTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor);
	HRESULT Add_MRT(wstring pMRTTag, wstring pTargetTag);

	HRESULT Begin_MRT(wstring pMRTTag);
	HRESULT Begin_Shadow(wstring pMRTTag);
	HRESULT Begin_Effects(wstring pMRTTag);
	HRESULT Begin_SSD(wstring pMRTTag);
	HRESULT End_MRT();
	HRESULT End_Shadow();

#ifdef _DEBUG
public:
	HRESULT Ready_Debug(wstring pTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug(wstring pMRTTag, class CShader* pShader, class CMesh_Rect* pVIBuffer);
	HRESULT Render_Shadow_Debug(wstring pMRTTag, class CShader* pShader, class CMesh_Rect* pVIBuffer, _float4x4 worldMatrix);
#endif // _DEBUG

private:
	/* 렌더타겟들을 모아놓는다. */
	map<_hashcode, class CRenderTarget*>			m_RenderTargets;
	typedef map<_hashcode, class CRenderTarget*>	RENDERTARGETS;

	/* 동시에 바인딩되어야하는 렌더타겟들을 그룹지어놓는다. */
	map<_hashcode, list<class CRenderTarget*>>				m_MRTs;
	typedef map<_hashcode, list<class CRenderTarget*>>		MRTS;

private:
	ComPtr<ID3D11RenderTargetView> m_pBackBufferView = nullptr;
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView = nullptr;
	ComPtr<ID3D11DepthStencilView> m_pShadowDSV = nullptr;
	D3D11_VIEWPORT					m_OriginViewPort;
	D3D11_VIEWPORT					m_ShadowViewPort;
public:
	class CRenderTarget* Find_RenderTarget(wstring pTargetTag);
	ComPtr<ID3D11RenderTargetView> Get_BackBuffer() { return m_pBackBufferView; }
	list<class CRenderTarget*>* Find_MRT(wstring pMRTTag);


public:
	virtual void Release();
};

END