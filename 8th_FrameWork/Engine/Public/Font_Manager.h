#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CFont_Manager final
{
	DECLARE_SINGLETON(CFont_Manager)

private:
	CFont_Manager();
	~CFont_Manager();

public:
	HRESULT Add_Font(const _tchar* pFontTag, const _tchar* pFontFilePath);
	HRESULT Render_Font(const _tchar* pFontTag, const _tchar* pString, const _float2& vPosition, const _float4& vColor);

private:
	map<_hashcode, class CCustomFont*>			m_Fonts;
	typedef map<const _tchar*, class CCustomFont*>	FONTS;

private:
	class CCustomFont* Find_Font(const _tchar* pFontTag);

public:
	void Release();
};

END