#include "..\Public\Texture.h"
#include "GameInstance.h"

#include "CShader.h"

#include "Renderer.h"

CTexture::CTexture(_uint iGroupIdx)
	: CComponent(iGroupIdx)
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

	return pShader->Set_ShaderResourceView(pConstantName, m_SRVs[m_iCurTextureIndex].pSRV);
}

HRESULT CTexture::Add_Texture(const _tchar* pTextureFilePath)
{
	TEXTUREDESC	tDesc;

	_tchar		szTextureFilePath[MAX_PATH] = TEXT("");
	ComPtr<ID3D11ShaderResourceView> pSRV = nullptr;

	wsprintf(szTextureFilePath, pTextureFilePath);

	_tchar			szExt[MAX_PATH] = TEXT("");

	_wsplitpath_s(szTextureFilePath, nullptr, 0, nullptr, 0, nullptr, 0, szExt, MAX_PATH);

	HRESULT		hr = 0;

	if (!lstrcmp(szExt, TEXT(".dds")))
		hr = DirectX::CreateDDSTextureFromFile(PDEVICE, szTextureFilePath, nullptr, pSRV.GetAddressOf());
	else if (!lstrcmp(szExt, TEXT(".tga")))
		hr = CreateTGATextureFromFile(szTextureFilePath, pSRV.GetAddressOf());
	else
		hr = DirectX::CreateWICTextureFromFile(PDEVICE, szTextureFilePath, nullptr, pSRV.GetAddressOf());

	if (FAILED(hr))
		return E_FAIL;

	tDesc.pSRV = pSRV;
	tDesc.strFilePath = szTextureFilePath;

	m_SRVs.push_back(tDesc);

	return S_OK;
}

void CTexture::Pop_Texture()
{
	if (m_SRVs.size() == 1)
		return;

	if (m_iCurTextureIndex == m_SRVs.size() - 1)
	{
		m_iCurTextureIndex--;
	}

	m_SRVs.pop_back();
}

_bool CTexture::Next_Texture()
{
	if (++m_iCurTextureIndex >= m_SRVs.size())
	{
		--m_iCurTextureIndex;
		return false;
	}

	return true;
}

void CTexture::Random_Texture()
{
	m_iCurTextureIndex = random(0, (_int)(m_SRVs.size() - 1));
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
		TEXTUREDESC	tDesc;

		ComPtr<ID3D11ShaderResourceView> pSRV = nullptr;

		wsprintf(szTextureFilePath, pTextureFilePath, i);

		_tchar			szExt[MAX_PATH] = TEXT("");

		_wsplitpath_s(szTextureFilePath, nullptr, 0, nullptr, 0, nullptr, 0, szExt, MAX_PATH);

		HRESULT		hr = 0;

		if (!lstrcmp(szExt, TEXT(".dds")))
			hr = DirectX::CreateDDSTextureFromFile(PDEVICE, szTextureFilePath, nullptr, pSRV.GetAddressOf());
		else if (!lstrcmp(szExt, TEXT(".tga")))
			hr = CreateTGATextureFromFile(szTextureFilePath, pSRV.GetAddressOf());
		else
			hr = DirectX::CreateWICTextureFromFile(PDEVICE, szTextureFilePath, nullptr, pSRV.GetAddressOf());

		if (FAILED(hr))
			return E_FAIL;

		tDesc.pSRV = pSRV;
		tDesc.strFilePath = szTextureFilePath;

		m_SRVs.push_back(tDesc);

	}

	return S_OK;
}

HRESULT CTexture::CreateTGATextureFromFile(const _tchar* pTextureFilePath, ID3D11ShaderResourceView** ppOutSRV)
{
	ScratchImage	tScImage;
	TexMetadata	tMetaData;
	if (FAILED(DirectX::LoadFromTGAFile(pTextureFilePath, &tMetaData, tScImage)))
		return E_FAIL;

	if (FAILED(CreateShaderResourceView(PDEVICE, tScImage.GetImages(), tScImage.GetImageCount(), tMetaData, ppOutSRV)))
		return E_FAIL;

	return S_OK;
}
