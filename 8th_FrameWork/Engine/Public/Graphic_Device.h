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
	ID3D11Device*			Get_Device(void) { return m_pDevice; }
	ID3D11DeviceContext*	Get_DeviceContext(void) { return m_pDeviceContext; }

public:
	HRESULT Ready_Graphic_Device(HWND hWnd, GRAPHICDESC::WINMODE WinMode, _uint iWinCX, _uint iWinCY, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut);
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT Clear_DepthStencil_View();
	HRESULT Present();


	HRESULT	Release();

private:
	HWND					m_mainhWnd = NULL;

	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;
	IDXGISwapChain*			m_pSwapChain = nullptr;

	// ShaderResourceView
	ID3D11RenderTargetView* m_pBackBufferRTV = nullptr;
	ID3D11DepthStencilView* m_pDepthStencilView = nullptr;

private:
	HRESULT Ready_SwapChain(HWND hWnd, GRAPHICDESC::WINMODE WinMode, _uint iWinCX, _uint iWinCY);
	HRESULT Ready_BackBufferRenderTargetView();
	HRESULT Ready_DepthStencilRenderTargetView(_uint iWinCX, _uint iWinCY);
};

END