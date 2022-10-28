#include "..\Public\CTarget_Manager.h"

#include "GameInstance.h"
#include "CRenderTarget.h"
#include "CMesh_Rect.h"

IMPLEMENT_SINGLETON(CTarget_Manager)

CTarget_Manager::CTarget_Manager()
{
}

CTarget_Manager::~CTarget_Manager()
{
	Release();
	m_pShadowDSV.Reset();

}

ComPtr<ID3D11ShaderResourceView> CTarget_Manager::Get_SRV(wstring pTargetTag)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(pTargetTag);

	if (nullptr == pRenderTarget)
		return nullptr;

	return pRenderTarget->Get_SRV();
}

void CTarget_Manager::Set_ShadowDSV(ComPtr<ID3D11DepthStencilView> pDSV, D3D11_VIEWPORT shadowViewPort)
{
	m_ShadowViewPort = shadowViewPort;
	m_pShadowDSV = pDSV;
}

HRESULT CTarget_Manager::Add_RenderTarget(wstring pTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor)
{
	if (nullptr != Find_RenderTarget(pTargetTag))
		return E_FAIL;

	CRenderTarget* pRenderTarget = CRenderTarget::Create(iWidth, iHeight, eFormat, vClearColor);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	m_RenderTargets.emplace(Convert_ToHash(pTargetTag), pRenderTarget);

	return S_OK;
}

HRESULT CTarget_Manager::Add_MRT(wstring pMRTTag, wstring pTargetTag)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(pTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	list<CRenderTarget*>* pMRTList = Find_MRT(pMRTTag);

	if (nullptr == pMRTList)
	{
		list<CRenderTarget*>		MRTList;
		MRTList.push_back(pRenderTarget);

		m_MRTs.emplace(Convert_ToHash(pMRTTag), MRTList);
	}
	else
	{
		pMRTList->push_back(pRenderTarget);
	}


	return S_OK;
}

HRESULT CTarget_Manager::Begin_MRT(wstring pMRTTag)
{
	list<CRenderTarget*>* pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	if (pMRTList->size() >= 8)
		return E_FAIL;

	ID3D11ShaderResourceView* pSRVs[128] = { nullptr };

	DEVICE_CONTEXT->PSSetShaderResources(0, 128, pSRVs);


	ID3D11RenderTargetView* RTVs[8] = { nullptr };

	_uint		iNumRTVs = 0;

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTarget->Clear();
		RTVs[iNumRTVs++] = pRenderTarget->Get_RTV().Get();
	}

	/* 기존에 바인딩되어있던(백버퍼 + 깊이스텐실버퍼)를 얻어온다. */
	DEVICE_CONTEXT->OMGetRenderTargets(1, m_pBackBufferView.GetAddressOf(), m_pDepthStencilView.GetAddressOf());

	/* 렌더타겟들을 장치에 바인딩한다. */
	DEVICE_CONTEXT->OMSetRenderTargets(iNumRTVs, RTVs, m_pDepthStencilView.Get());

	return S_OK;
}

HRESULT CTarget_Manager::Begin_Shadow(wstring pMRTTag)
{

	list<CRenderTarget*>* pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	if (pMRTList->size() >= 8)
		return E_FAIL;

	ID3D11ShaderResourceView* pSRVs[128] = { nullptr };

	DEVICE_CONTEXT->PSSetShaderResources(0, 128, pSRVs);


	ID3D11RenderTargetView* RTVs[8] = { nullptr };

	_uint		iNumRTVs = 0;

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTarget->Clear();
		RTVs[iNumRTVs++] = pRenderTarget->Get_RTV().Get();
	}
	DEVICE_CONTEXT->ClearDepthStencilView(m_pShadowDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	/* 기존에 바인딩되어있던(백버퍼 + 깊이스텐실버퍼)를 얻어온다. */
	DEVICE_CONTEXT->OMGetRenderTargets(1, m_pBackBufferView.GetAddressOf(), m_pDepthStencilView.GetAddressOf());

	/* 렌더타겟들을 장치에 바인딩한다. */
	if (!m_pShadowDSV)
		return E_FAIL;

	DEVICE_CONTEXT->OMSetRenderTargets(iNumRTVs, RTVs, m_pShadowDSV.Get());
	_uint		iNumViewports = 1;

	DEVICE_CONTEXT->RSGetViewports(&iNumViewports, &m_OriginViewPort);
	DEVICE_CONTEXT->RSSetViewports(1, &m_ShadowViewPort);

	return S_OK;
}

HRESULT CTarget_Manager::Begin_Effects(wstring pMRTTag)
{
	list<CRenderTarget*>* pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	if (pMRTList->size() >= 8)
		return E_FAIL;

	ID3D11ShaderResourceView* pSRVs[128] = { nullptr };

	DEVICE_CONTEXT->PSSetShaderResources(0, 128, pSRVs);


	ID3D11RenderTargetView* RTVs[8] = { nullptr };

	_uint		iNumRTVs = 0;

	for (auto& pRenderTarget : *pMRTList)
	{
		RTVs[iNumRTVs++] = pRenderTarget->Get_RTV().Get();
		if (iNumRTVs > 2)
		{
			pRenderTarget->Clear();

		}
	}


	/* 기존에 바인딩되어있던(백버퍼 + 깊이스텐실버퍼)를 얻어온다. */
	DEVICE_CONTEXT->OMGetRenderTargets(1, m_pBackBufferView.GetAddressOf(), m_pDepthStencilView.GetAddressOf());

	/* 렌더타겟들을 장치에 바인딩한다. */
	DEVICE_CONTEXT->OMSetRenderTargets(iNumRTVs, RTVs, m_pDepthStencilView.Get());

	return S_OK;
}

HRESULT CTarget_Manager::Begin_SSD(wstring pMRTTag)
{
	list<CRenderTarget*>* pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	if (pMRTList->size() >= 8)
		return E_FAIL;

	ID3D11ShaderResourceView* pSRVs[8] = { nullptr };

	DEVICE_CONTEXT->PSSetShaderResources(0, 8, pSRVs);


	ID3D11RenderTargetView* RTVs[8] = { nullptr };

	_uint		iNumRTVs = 0;

	for (auto& pRenderTarget : *pMRTList)
	{
		RTVs[iNumRTVs++] = pRenderTarget->Get_RTV().Get();
	}


	/* 기존에 바인딩되어있던(백버퍼 + 깊이스텐실버퍼)를 얻어온다. */
	DEVICE_CONTEXT->OMGetRenderTargets(1, m_pBackBufferView.GetAddressOf(), m_pDepthStencilView.GetAddressOf());

	/* 렌더타겟들을 장치에 바인딩한다. */
	DEVICE_CONTEXT->OMSetRenderTargets(iNumRTVs, RTVs, m_pDepthStencilView.Get());

	return S_OK;
}

HRESULT CTarget_Manager::End_MRT()
{

	DEVICE_CONTEXT->OMSetRenderTargets(1, m_pBackBufferView.GetAddressOf(), m_pDepthStencilView.Get());
	m_pBackBufferView.Reset();
	m_pDepthStencilView.Reset();

	return S_OK;
}

HRESULT CTarget_Manager::End_Shadow()
{

	DEVICE_CONTEXT->OMSetRenderTargets(1, m_pBackBufferView.GetAddressOf(), m_pDepthStencilView.Get());
	DEVICE_CONTEXT->RSSetViewports(1, &m_OriginViewPort);

	m_pDepthStencilView.Reset();

	m_pBackBufferView.Reset();
	//m_pShadowDSV.Reset();

	return S_OK;
}

#ifdef _DEBUG

HRESULT CTarget_Manager::Ready_Debug(wstring pTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(pTargetTag);

	if (nullptr == pRenderTarget)
		return E_FAIL;

	return pRenderTarget->Ready_Debug(fX, fY, fSizeX, fSizeY);
}

HRESULT CTarget_Manager::Render_Debug(wstring pMRTTag, CShader* pShader, CMesh_Rect* pVIBuffer)
{
	list<class CRenderTarget*>* pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTarget->Render_Debug(pShader, pVIBuffer);
	}

	return S_OK;
}
HRESULT CTarget_Manager::Render_Shadow_Debug(wstring pMRTTag, CShader* pShader, CMesh_Rect* pVIBuffer, _float4x4 worldMatrix)
{
	list<class CRenderTarget*>* pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	pMRTList->front()->Render_Shadow_Debug(pShader, pVIBuffer, worldMatrix);
	

	return S_OK;
}
#endif // _DEBUG

CRenderTarget* CTarget_Manager::Find_RenderTarget(wstring pTargetTag)
{
	auto	iter = m_RenderTargets.find(Convert_ToHash(pTargetTag));

	if (iter == m_RenderTargets.end())
		return nullptr;

	return iter->second;
}

list<class CRenderTarget*>* CTarget_Manager::Find_MRT(wstring pMRTTag)
{
	auto	iter = m_MRTs.find(Convert_ToHash(pMRTTag));

	if (iter == m_MRTs.end())
		return nullptr;

	return &iter->second;
}

void CTarget_Manager::Release()
{
	for (auto& Pair : m_RenderTargets)
	{
		SAFE_DELETE(Pair.second);
	}
	m_RenderTargets.clear();

	m_MRTs.clear();

	_long dwCnt = 0;

	if (m_pBackBufferView)
	dwCnt = m_pBackBufferView.Reset();
	if (m_pDepthStencilView)
	dwCnt = m_pDepthStencilView.Reset();


}
