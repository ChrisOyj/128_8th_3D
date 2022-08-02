#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CGraphic_Device
{
	DECLARE_SINGLETON(CGraphic_Device)

private:
	CGraphic_Device();
	~CGraphic_Device();

public:
	ComPtr<ID3D11Device>			Get_Device(void) { return m_pDevice; }
	ComPtr<ID3D11DeviceContext>		Get_DeviceContext(void) { return m_pDeviceContext; }

public:
	HRESULT Ready_Graphic_Device(const GRAPHICDESC& GraphicDesc);
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT Clear_DepthStencil_View();
	HRESULT Present();


	HRESULT	Release();

private:
	HWND					m_mainhWnd = NULL;

	ComPtr<ID3D11Device>			m_pDevice;
	ComPtr<ID3D11DeviceContext>		m_pDeviceContext;
	ComPtr<ID3D11DeviceContext>		m_pDeviceContext2;
	ComPtr<IDXGISwapChain>			m_pSwapChain;

	// ShaderResourceView
	ComPtr<ID3D11RenderTargetView> m_pBackBufferRTV;
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;

private:
	HRESULT Ready_SwapChain(HWND hWnd, GRAPHICDESC::WINMODE WinMode, _uint iWinCX, _uint iWinCY);
	HRESULT Ready_BackBufferRenderTargetView();
	HRESULT Ready_DepthStencilRenderTargetView(_uint iWinCX, _uint iWinCY);
};

END