#include "..\Public\Font_Manager.h"
#include "CustomFont.h"

IMPLEMENT_SINGLETON(CFont_Manager)

CFont_Manager::CFont_Manager()
{
}

CFont_Manager::~CFont_Manager()
{
	Release();
}

HRESULT CFont_Manager::Add_Font(const _tchar* pFontTag, const _tchar * pFontFilePath)
{
	if (nullptr != Find_Font(pFontTag))
		return E_FAIL;

	CCustomFont*		pFont = CCustomFont::Create(pFontFilePath);

	if (nullptr == pFont)
		return E_FAIL;

	m_Fonts.emplace(Convert_ToHash(pFontTag), pFont);

	return S_OK;
}

HRESULT CFont_Manager::Render_Font(const _tchar * pFontTag, const _tchar * pString, const _float2 & vPosition, const _float4& vColor)
{
	CCustomFont*		pFont = Find_Font(pFontTag);
	if (nullptr == pFont)
		return E_FAIL;

	return pFont->Render(pString, vPosition, vColor);	
}

CCustomFont * CFont_Manager::Find_Font(const _tchar * pFontTag)
{
	auto	iter = m_Fonts.find(Convert_ToHash(pFontTag));

	if (iter == m_Fonts.end())
		return nullptr;

	return iter->second;
}

void CFont_Manager::Release()
{
	for (auto& Pair : m_Fonts)
		SAFE_DELETE(Pair.second);

	m_Fonts.clear();
}
