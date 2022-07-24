#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CTexture final
{
public:
	CTexture();
	CTexture(const CTexture& _origin);
	~CTexture();

public:
	virtual CTexture*	Clone() { return new CTexture(*this); };

public:
	LPDIRECT3DBASETEXTURE9	Get_Texture(const _uint& iTextureIndex = 0) { return m_vecTexture[iTextureIndex]; }
	_uint					Get_Width(const _uint& iTextureIndex = 0) { return m_vecImgInfo[iTextureIndex].Width; }
	_uint					Get_Height(const _uint& iTextureIndex = 0) { return m_vecImgInfo[iTextureIndex].Height; }

public:
	HRESULT	Initialize(TEX_TYPE eType, const _tchar* pFilePath, const _uint& iNumTextures);

public:
	HRESULT	Bind_Texture(const _uint& iTextureIndex);

public:
	static CTexture* Create(TEX_TYPE eType, const _tchar* pFilePath, const _uint& iNumTextures);

private:
	vector<LPDIRECT3DBASETEXTURE9>		m_vecTexture;
	vector<D3DXIMAGE_INFO>				m_vecImgInfo;

	_uint								m_iNumTextures = 0;
	TEX_TYPE							m_eTexType;

private:
	friend class CMeshRenderer;
	friend class CUIRenderer;
	friend class CParticleSystem;
	void Release();

};
END
