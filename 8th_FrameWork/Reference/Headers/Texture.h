#pragma once
#include "Component.h"

BEGIN(Engine)

class CRenderer;
class CShader;

struct TEXTUREDESC
{
	ComPtr<ID3D11ShaderResourceView>	pSRV = nullptr;
	wstring	strFilePath;
};

class ENGINE_DLL CTexture
	: public CComponent
{
	DECLARE_PROTOTYPE(CTexture);

private:
	CTexture(_uint iGroupIdx);
	virtual ~CTexture();

public:
	static CTexture* Create(_uint iGroupIdx, const _tchar* pTextureFilePath, const _uint& iNumTextures);

public:
	void	Set_CurTextureIndex(const _uint& iIndex) { 
		if (iIndex < 0 || iIndex >= m_SRVs.size())
		{
			Call_MsgBox(L"Out of Range on Set_CurTextureIndex : CTexture");
		}
		m_iCurTextureIndex = iIndex;
	}
	_uint	Get_CurTextureIndex() { return m_iCurTextureIndex; }
	_uint	Get_TextureSize() { return (_uint)m_SRVs.size(); }
	vector<TEXTUREDESC>& Get_vecTexture() { return m_SRVs; }
	HRESULT Set_ShaderResourceView(class CShader* pShader, const char* pConstantName);

public:
	HRESULT	Add_Texture(const _tchar* pTextureFilePath);
	void	Pop_Texture();

public:
	_bool	Next_Texture();
	void	Random_Texture();

public:
	virtual void Tick() override;
	virtual void Late_Tick() override;

private:
	vector<TEXTUREDESC>			m_SRVs;

	_uint		m_iCurTextureIndex = 0;

private:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;

	/* Non_Update_Component */
	virtual void Start() override {}
	virtual void OnEnable() override {}
	virtual void OnDisable() override {}

	virtual void Release() override;

private:
	HRESULT		SetUp_Textures(const _tchar* pTextureFilePath, const _uint& iNumTextures);

};
END