#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CCustomFont final
{
	friend class CFont_Manager;
private:
	CCustomFont();
	~CCustomFont();

public:
	static CCustomFont* Create(const _tchar* pFontFilePath);

public:
	HRESULT Initialize(const _tchar* pFontFilePath);
	HRESULT Render(const _tchar* pString, const _float2& vPosition, const _float4& vColor);

private:
	SpriteBatch*				m_pBatch = nullptr;
	SpriteFont*					m_pFont = nullptr;

private:
	void Release();

};

END