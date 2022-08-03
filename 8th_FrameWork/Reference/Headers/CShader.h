#pragma once
#include "Component.h"

BEGIN(Engine)

class CTexture;
class CRenderer;

class ENGINE_DLL CShader final
	: public CComponent
{
	DECLARE_PROTOTYPE(CShader);

public:
	typedef struct tagPasses
	{
		ComPtr<ID3DX11EffectPass> pPass = nullptr;
		ComPtr<ID3D11InputLayout> pInputLayout = nullptr;
	}PASSDESC;

private:
	CShader(_uint iGroupIdx);
	CShader(const CShader& _origin);
	virtual ~CShader() = default;

public:
	static CShader* Create(_uint iGroupIdx, const _tchar* pShaderFilePath,
		const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElement);

public:
	virtual HRESULT SetUp_ShaderResources(const _uint& iPassIndex, CTexture* pTextureCom);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void	Tick() override;
	virtual void	Late_Tick() override;

public:
	HRESULT Set_RawValue(const char* pConstantName, void* pData, _uint iDataSize);
	HRESULT Set_ShaderResourceView(const char* pConstantName, ComPtr<ID3D11ShaderResourceView> pSRV);
	HRESULT Begin(_uint iPassIndex);

public:
	CDelegate<CShader*, const char*>	CallBack_SetRawValues;

private:
	ComPtr<ID3DX11Effect>			m_pEffect = nullptr;

	vector<PASSDESC>				m_Passes;
	typedef vector<PASSDESC>		PASSES;

private:
	/* Non_Update_Component */
	virtual void Start() override {}
	virtual void OnEnable() override {}
	virtual void OnDisable() override {}

	virtual	void	Release() override;

	HRESULT			SetUp_Shader(const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElement);



};

END