#include "..\Public\Texture.h"
#include "GameInstance.h"


CTexture::CTexture()
{
}

CTexture::CTexture(const CTexture& _origin)
	: m_vecTexture(_origin.m_vecTexture)
	, m_vecImgInfo(_origin.m_vecImgInfo)
	, m_eTexType(_origin.m_eTexType)
	, m_iNumTextures(_origin.m_iNumTextures)
{
	for (auto& elem : _origin.m_vecTexture)
		Safe_AddRef(elem);
}

CTexture::~CTexture()
{
	Release();
}

HRESULT CTexture::Initialize(TEX_TYPE eType, const _tchar * pFilePath, const _uint& iNumTextures)
{
	m_vecTexture.reserve(iNumTextures);
	m_iNumTextures = iNumTextures;
	m_eTexType = eType;
	_tchar		szTextureFullpPath[MAX_PATH] = L"";

	for (_uint i = 0; i < m_iNumTextures; ++i)
	{
		LPDIRECT3DBASETEXTURE9	pTexture = nullptr;

		wsprintf(szTextureFullpPath, pFilePath, i);

		HRESULT hr = 0;

		switch (m_eTexType)
		{
		case TEX_DEFAULT:
			hr = D3DXCreateTextureFromFile(DEVICE, szTextureFullpPath, (LPDIRECT3DTEXTURE9*)&pTexture);

			break;

		case TEX_CUBE:
			hr = D3DXCreateCubeTextureFromFile(DEVICE, szTextureFullpPath, (LPDIRECT3DCUBETEXTURE9*)&pTexture);

			break;

		default:
			break;
		}

		if (FAILED(hr))
			return E_FAIL;

		D3DXIMAGE_INFO	tInfo;

		if (FAILED(D3DXGetImageInfoFromFile(szTextureFullpPath, &tInfo)))
			return E_FAIL;

		m_vecImgInfo.push_back(tInfo);
		m_vecTexture.push_back(pTexture);

	}

	return S_OK;
}

HRESULT CTexture::Bind_Texture(const _uint& iTextureIndex)
{
	if (iTextureIndex >= m_iNumTextures)
		return E_FAIL;

	DEVICE->SetTexture(0, m_vecTexture[iTextureIndex]);

	return S_OK;
}

CTexture * CTexture::Create(TEX_TYPE eType, const _tchar * pFilePath, const _uint& iNumTextures)
{
	CTexture*	pTexture = new CTexture;
	if (FAILED(pTexture->Initialize(eType, pFilePath, iNumTextures)))
	{
		MSG_BOX("Failed to Initialize : CTexture");
		delete pTexture;

		return nullptr;
	}

	return pTexture;
}

void CTexture::Release()
{
	for (_uint i = 0; i < m_vecTexture.size(); ++i)
	{
		Safe_Release(m_vecTexture[i]);
	}
}
