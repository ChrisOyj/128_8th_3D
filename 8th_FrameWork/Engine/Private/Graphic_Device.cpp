#include "Graphic_Device.h"

CGraphic_Device::CGraphic_Device()
{

}

CGraphic_Device::~CGraphic_Device()
{
	Release();
}

HRESULT CGraphic_Device::Ready_Graphic_Device(const GRAPHICDESC& GraphicDesc)
{
	_uint		iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL			FeatureLV;

	/* 그래픽 장치를 초기화한다. */
	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, iFlag, nullptr, 0, D3D11_SDK_VERSION, m_pDevice.GetAddressOf(), &FeatureLV, m_pDeviceContext.GetAddressOf())))
		return E_FAIL;

	if (FAILED(Ready_SwapChain(GraphicDesc.hWnd, GraphicDesc.WinMode, GraphicDesc.iWinCX, GraphicDesc.iWinCY)))
		return E_FAIL;

	if (FAILED(Ready_BackBufferRenderTargetView()))
		return E_FAIL;

	if (FAILED(Ready_DepthStencilRenderTargetView(GraphicDesc.iWinCX, GraphicDesc.iWinCY)))
		return E_FAIL;

	/* 장치에 바인드해놓을 렌더타겟들과 뎁스스텐실뷰를 셋팅한다. */
	m_pDeviceContext->OMSetRenderTargets(1, m_pBackBufferRTV.GetAddressOf(), m_pDepthStencilView.Get());

	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	ViewPortDesc.TopLeftX = 0;
	ViewPortDesc.TopLeftY = 0;
	ViewPortDesc.Width = (_float)GraphicDesc.iWinCX;
	ViewPortDesc.Height = (_float)GraphicDesc.iWinCY;
	ViewPortDesc.MinDepth = 0.f;
	ViewPortDesc.MaxDepth = 1.f;

	m_pDeviceContext->RSSetViewports(1, &ViewPortDesc);

	return S_OK;
}

HRESULT CGraphic_Device::Clear_BackBuffer_View(_float4 vClearColor)
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;

	/* 백버퍼를 초기화하낟.  */
	m_pDeviceContext->ClearRenderTargetView(m_pBackBufferRTV.Get(), (_float*)&vClearColor);




	return S_OK;
}

HRESULT CGraphic_Device::Clear_DepthStencil_View()
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;

	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	return S_OK;
}

HRESULT CGraphic_Device::Present()
{
	if (nullptr == m_pSwapChain)
		return E_FAIL;

	return m_pSwapChain->Present(0, 0);
}


HRESULT CGraphic_Device::Ready_SwapChain(HWND hWnd, GRAPHICDESC::WINMODE eWinMode, _uint iWinCX, _uint iWinCY)
{
	ComPtr<IDXGIDevice> pDevice = nullptr;
	m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)pDevice.GetAddressOf());

	ComPtr < IDXGIAdapter> pAdapter = nullptr;
	pDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf());

	ComPtr < IDXGIFactory> pFactory = nullptr;
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf());

	DXGI_SWAP_CHAIN_DESC		SwapChain;
	ZeroMemory(&SwapChain, sizeof(DXGI_SWAP_CHAIN_DESC));

	SwapChain.BufferDesc.Width = iWinCX;
	SwapChain.BufferDesc.Height = iWinCY;
	SwapChain.BufferDesc.RefreshRate.Numerator = 60;
	SwapChain.BufferDesc.RefreshRate.Denominator = 1;
	SwapChain.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	SwapChain.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChain.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	SwapChain.SampleDesc.Quality = 0;
	SwapChain.SampleDesc.Count = 1;
	SwapChain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChain.BufferCount = 1;
	SwapChain.OutputWindow = hWnd;
	SwapChain.Windowed = eWinMode;
	SwapChain.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	if (FAILED(pFactory->CreateSwapChain(m_pDevice.Get(), &SwapChain, m_pSwapChain.GetAddressOf())))
		return E_FAIL;

	//Safe_Release(pFactory);
	//Safe_Release(pAdapter);
	//Safe_Release(pDevice);

	return S_OK;
}


HRESULT CGraphic_Device::Ready_BackBufferRenderTargetView()
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	ComPtr < ID3D11Texture2D> pBackBufferTexture = nullptr;

	if (FAILED(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)pBackBufferTexture.GetAddressOf())))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateRenderTargetView(pBackBufferTexture.Get(), nullptr, m_pBackBufferRTV.GetAddressOf())))
		return E_FAIL;

	//Safe_Release(pBackBufferTexture);

	return S_OK;
}

HRESULT CGraphic_Device::Ready_DepthStencilRenderTargetView(_uint iWinCX, _uint iWinCY)
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	ComPtr < ID3D11Texture2D> pDepthStencilTexture = nullptr;

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = iWinCX;
	TextureDesc.Height = iWinCY;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, pDepthStencilTexture.GetAddressOf())))
		return E_FAIL;

	/* RenderTarget */
	/* ShaderResource */
	/* DepthStencil */

	if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture.Get(), nullptr, m_pDepthStencilView.GetAddressOf())))
		return E_FAIL;

	//Safe_Release(pDepthStencilTexture);

	return S_OK;
}



HRESULT CGraphic_Device::Release()
{
	_ulong dwCnt = 0;

	dwCnt = m_pSwapChain.Reset();
	dwCnt = m_pBackBufferRTV.Reset();
	dwCnt = m_pDepthStencilView.Reset();
	dwCnt = m_pDeviceContext.Reset();

	//m_pDeviceContext.ReleaseAndGetAddressOf();
	dwCnt = m_pDevice.Reset();

	return S_OK;
}