#include "..\Public\CustomFont.h"

#include "GameInstance.h"

CCustomFont::CCustomFont()
{
}

CCustomFont::~CCustomFont()
{
	Release();
}

CCustomFont* CCustomFont::Create(const _tchar* pFontFilePath)
{
	CCustomFont* pInstance = new CCustomFont();

	if (FAILED(pInstance->Initialize(pFontFilePath)))
	{
		Call_MsgBox(L"Failed to Initialize : CCustomFont");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CCustomFont::Initialize(const _tchar * pFontFilePath)
{
	m_pBatch = new SpriteBatch(PDEVICE_CONTEXT);
	m_pFont = new SpriteFont(PDEVICE, pFontFilePath);

	if (!m_pBatch || !m_pFont)
		return E_FAIL;

	return S_OK;
}



HRESULT CCustomFont::Render(const _tchar* pString, const _float2& vPosition, const _float4& vColor)
{
	if (nullptr == m_pBatch ||
		nullptr == m_pFont)
		return E_FAIL;

	m_pBatch->Begin();

	m_pFont->DrawString(m_pBatch, pString, vPosition, XMLoadFloat4(&vColor));

	m_pBatch->End();

	return S_OK;
}





void CCustomFont::Release()
{
	SAFE_DELETE(m_pBatch);
	SAFE_DELETE(m_pFont);
}