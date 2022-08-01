#include "..\Public\Texture.h"
#include "GameInstance.h"

#include "CShader.h"

#include "Renderer.h"

CTexture::CTexture(_uint iGroupIdx)
	: CComponent(iGroupIdx)
{
}

CTexture::CTexture(const CTexture& _origin)
	: CComponent(_origin)
{
}

CTexture::~CTexture()
{
	
}
CTexture* CTexture::Create(_uint iGroupIdx, const _tchar* pTextureFilePath, const _uint& iNumTextures)
{
	CTexture* pTexture = new CTexture(iGroupIdx);

	if (FAILED(pTexture->SetUp_Textures(pTextureFilePath, iNumTextures)))
	{
		SAFE_DELETE(pTexture);
		Call_MsgBox(L"Failed to Initialize_Prototype : CTexture");
		return nullptr;
	}

	if (FAILED(pTexture->Initialize_Prototype()))
	{
		SAFE_DELETE(pTexture);
		Call_MsgBox(L"Failed to Initialize_Prototype : CTexture");
		return nullptr;
	}

	return pTexture;
}

HRESULT CTexture::Set_ShaderResourceView(CShader* pShader, const char* pConstantName)
{
	if (m_iCurTextureIndex >= m_SRVs.size())
		return E_FAIL;

	return pShader->Set_ShaderResourceView(pConstantName, m_SRVs[m_iCurTextureIndex]);
}

void CTexture::Tick()
{
}

void CTexture::Late_Tick()
{
}

HRESULT CTexture::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTexture::Initialize()
{
	return S_OK;
}

void CTexture::Release()
{
}

HRESULT CTexture::SetUp_Textures(const _tchar* pTextureFilePath, const _uint& iNumTextures)
{
	_tchar		szTextureFilePath[MAX_PATH] = TEXT("");

	m_SRVs.reserve(iNumTextures);

	for (_uint i = 0; i < iNumTextures; ++i)
	{
		ID3D11ShaderResourceView* pSRV = nullptr;

		wsprintf(szTextureFilePath, pTextureFilePath, i);

		_tchar			szExt[MAX_PATH] = TEXT("");

		_wsplitpath_s(szTextureFilePath, nullptr, 0, nullptr, 0, nullptr, 0, szExt, MAX_PATH);

		HRESULT		hr = 0;

		if (!lstrcmp(szExt, TEXT(".dds")))
			hr = DirectX::CreateDDSTextureFromFile(DEVICE, szTextureFilePath, nullptr, &pSRV);

		else
			hr = DirectX::CreateWICTextureFromFile(DEVICE, szTextureFilePath, nullptr, &pSRV);

		if (FAILED(hr))
			return E_FAIL;

		m_SRVs.push_back(pSRV);

	}

	return S_OK;
}
