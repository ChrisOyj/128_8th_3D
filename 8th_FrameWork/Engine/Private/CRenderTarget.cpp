#include "..\Public\CRenderTarget.h"
#include "GameInstance.h"
#include "CShader.h"
#include "CMesh_Rect.h"

CRenderTarget::CRenderTarget()
{
}

CRenderTarget::~CRenderTarget()
{
	Release();
}

CRenderTarget* CRenderTarget::Create(_uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor)
{
	CRenderTarget* pInstance = new CRenderTarget;

	if (FAILED(pInstance->Initialize(iWidth, iHeight, eFormat, vClearColor)))
	{
		Call_MsgBox(L"Failed to Created : CRenderTarget");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CRenderTarget::Initialize(_uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor)
{
	m_vClearColor = vClearColor;


	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = iWidth;
	TextureDesc.Height = iHeight;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = eFormat;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(DEVICE->CreateTexture2D(&TextureDesc, nullptr, m_pTexture.GetAddressOf())))
		return E_FAIL;

	if (FAILED(DEVICE->CreateRenderTargetView(m_pTexture.Get(), nullptr, m_pRTV.GetAddressOf())))
		return E_FAIL;

	if (FAILED(DEVICE->CreateShaderResourceView(m_pTexture.Get(), nullptr, m_pSRV.GetAddressOf())))
		return E_FAIL;





	return S_OK;
}

HRESULT CRenderTarget::Clear()
{
	/*if (m_vClearColor.x == 1.f)
		DirectX::SaveDDSTextureToFile(m_pContext, m_pTexture, TEXT("../Bin/Diffuse.dds"));
	if (m_vClearColor.y == 1.f)
		DirectX::SaveDDSTextureToFile(m_pContext, m_pTexture, TEXT("../Bin/Normal.dds"));

*/
	DEVICE_CONTEXT->ClearRenderTargetView(m_pRTV.Get(), (_float*)&m_vClearColor);


	return S_OK;
}

#ifdef _DEBUG

HRESULT CRenderTarget::Ready_Debug(_float fX, _float fY, _float fSizeX, _float fSizeY)
{

	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));

	_uint		iNumViewports = 1;

	DEVICE_CONTEXT->RSGetViewports(&iNumViewports, &ViewPortDesc);

	_matrix		WorldMatrix = XMMatrixIdentity();

	WorldMatrix.r[0] = XMVectorSet(fSizeX, 0.f, 0.f, 0.f);
	WorldMatrix.r[1] = XMVectorSet(0.f, fSizeY, 0.f, 0.f);
	WorldMatrix.r[3] = XMVectorSet(fX - (ViewPortDesc.Width * 0.5f), -fY + (ViewPortDesc.Height * 0.5f), 0.f, 1.f);

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixTranspose(WorldMatrix));

	return S_OK;
}

HRESULT CRenderTarget::Render_Debug(CShader* pShader, CMesh_Rect* pVIBuffer)
{
	/* 직교투영을 위한 정보를 던진다. */
	if (FAILED(pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(pShader->Set_ShaderResourceView("g_Texture", m_pSRV)))
		return E_FAIL;

	if (FAILED(pShader->Begin(0)))
		return E_FAIL;

	return pVIBuffer->Render();
}

HRESULT CRenderTarget::Render_Shadow_Debug(CShader* pShader, CMesh_Rect* pVIBuffer, _float4x4 worldMat)
{
	/* 직교투영을 위한 정보를 던진다. */
	if (FAILED(pShader->Set_RawValue("g_WorldMatrix", &worldMat, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(pShader->Set_ShaderResourceView("g_Texture", m_pSRV)))
		return E_FAIL;

	if (FAILED(pShader->Begin(0)))
		return E_FAIL;

	return pVIBuffer->Render();
}

#endif // _DEBUG



void CRenderTarget::Release()
{
	_long dwCnt = 0;
	dwCnt = m_pSRV.Reset();
	dwCnt = m_pRTV.Reset();
	dwCnt = m_pTexture.Reset();
}
