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
	virtual ~CShader();

	friend class CRender_Manager;

public:
	static CShader* Create(_uint iGroupIdx, const _uint& iShaderFileIdx,
		const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElement);

	static CShader* Create(const _tchar* pFilePath,
		const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElement);

public:
	_uint	Get_ShaderFileIndex() { return m_iShaderFileIdx; }

public:
	virtual HRESULT SetUp_ShaderResources(CTexture* pTextureCom, const char* pConstantName);

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
	CDelegate<const char*, CTexture*>	CallBack_SetSRVs;

private:
	ComPtr<ID3DX11Effect>			m_pEffect = nullptr;

	vector<PASSDESC>				m_Passes;
	typedef vector<PASSDESC>		PASSES;

	_uint							m_iShaderFileIdx = 0;

private:
	/* Non_Update_Component */
	virtual void Start() override {}
	virtual void OnEnable() override {}
	virtual void OnDisable() override {}

	virtual	void	Release() override;

	HRESULT			SetUp_Shader(const _uint& iShaderFileIdx, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElement);
	HRESULT			SetUp_Shader(const _tchar* pFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElement);



};

END