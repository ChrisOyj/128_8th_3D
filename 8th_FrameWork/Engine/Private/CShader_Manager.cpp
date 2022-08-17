#include "CShader_Manager.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CShader_Manager)


CShader_Manager::CShader_Manager()
{}

CShader_Manager::~CShader_Manager()
{}

HRESULT CShader_Manager::Load_EffectFile(const _tchar * pFilePath)
{
	ComPtr < ID3DX11Effect> pEffect;

	_uint		iHLSLFlag = 0;

#ifdef _DEBUG
	iHLSLFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_OPTIMIZATION_LEVEL0;
#else
	iHLSLFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif

	if (FAILED(D3DX11CompileEffectFromFile(pFilePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		iHLSLFlag, 0, PDEVICE, pEffect.GetAddressOf(), /*&pError*/nullptr)))
		return E_FAIL;

	Add_Effect(pEffect);

	return S_OK;
}

ComPtr<ID3DX11Effect> CShader_Manager::Get_EffectFile(const _uint& iIndex)
{
	if (iIndex < 0 || iIndex > m_vecEffects.size())
	{
		Call_MsgBox(L"Out of Range in Get_EffectFile : CShdaer_Manager");
		return nullptr;
	}

	return m_vecEffects[iIndex];
}

HRESULT CShader_Manager::Add_Effect(ComPtr<ID3DX11Effect> pEffect)
{
	for (auto& pEffectOrigin : m_vecEffects)
	{
		if (pEffectOrigin == pEffect)
			return S_OK;
	}

	m_vecEffects.push_back(pEffect);


	return S_OK;
}

HRESULT CShader_Manager::Set_RawValue(const _uint& iIndex, const char* pConstantName, void* pData, _uint iDataSize)
{
	if (iIndex < 0 || iIndex >= m_vecEffects.size())
		return E_FAIL;

	ID3DX11EffectVariable* pVariable = m_vecEffects[iIndex]->GetVariableByName(pConstantName);

	if (nullptr == pVariable)
		return E_FAIL;

	return pVariable->SetRawValue(pData, 0, iDataSize);
}

HRESULT CShader_Manager::Set_RawValue_All(const char* pConstantName, void* pData, _uint iDataSize)
{
	for (_uint i = 0; i < m_vecEffects.size(); ++i)
	{
		ID3DX11EffectVariable* pVariable = m_vecEffects[i]->GetVariableByName(pConstantName);

		if (nullptr == pVariable)
			return E_FAIL;

		if (FAILED(pVariable->SetRawValue(pData, 0, iDataSize)))
			return E_FAIL;
	}
	
	return S_OK;
}
