#include "CShader_Manager.h"

IMPLEMENT_SINGLETON(CShader_Manager)


CShader_Manager::CShader_Manager()
{}

CShader_Manager::~CShader_Manager()
{}

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
