#pragma once
#include "Component.h"

BEGIN(Engine)

class CRenderer;
class CShader;

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
	void	Set_CurTextureIndex(const _uint& iIndex) { m_iCurTextureIndex = iIndex; }
	HRESULT Set_ShaderResourceView(class CShader* pShader, const char* pConstantName);

public:
	virtual void Tick() override;
	virtual void Late_Tick() override;

private:
	vector<ComPtr<ID3D11ShaderResourceView>>			m_SRVs;
	typedef vector<ComPtr<ID3D11ShaderResourceView>>	SRVS;

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