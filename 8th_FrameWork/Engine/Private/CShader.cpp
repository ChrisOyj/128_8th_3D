#include "..\Public\CShader.h"
#include "CShader.h"

#include "GameObject.h"

#include "GameInstance.h"

#include "Texture.h"
#include "Transform.h"

CShader::CShader(CGameObject* pOwner)
	: CComponent(pOwner)
{
}

CShader::CShader(const CShader& rhs)
	: CComponent(rhs)
	, m_pEffect(rhs.m_pEffect)
	, m_Passes(rhs.m_Passes)
{
}

CShader* CShader::Create(CGameObject* pOwner, const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElement)
{
	CShader* pInstance = new CShader(pOwner);

	if (FAILED(pInstance->SetUp_Shader(pShaderFilePath, pElements, iNumElement)))
	{
		Call_MsgBox(L"Failed to SetUp_Shader CShader : CShader");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to SetUp_Shader CShader : CShader");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	return pInstance;
}

//만약 이녀석이 그냥 call_back 함수여서 등록된거 존나 call 해
HRESULT CShader::SetUp_ShaderResources(const _uint& iPassIndex, CTexture* pTextureCom)
{
	if (FAILED(pTextureCom->Set_ShaderResourceView(this, "MyTex")))
		return E_FAIL;

	CallBack_SetRawValues(this, "");

	return S_OK;
}

HRESULT CShader::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CShader::Initialize()
{
	return S_OK;
}

void CShader::Tick()
{
}

void CShader::Late_Tick()
{
}

HRESULT CShader::Set_RawValue(const char* pConstantName, void* pData, _uint iDataSize)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	return pVariable->SetRawValue(pData, 0, iDataSize);

}

HRESULT CShader::Set_ShaderResourceView(const char* pConstantName, ComPtr<ID3D11ShaderResourceView> pSRV)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectShaderResourceVariable* pVariable_SRV = pVariable->AsShaderResource();

	return pVariable_SRV->SetResource(pSRV.Get());
}

HRESULT CShader::Begin(_uint iPassIndex)
{
	if (iPassIndex >= m_Passes.size())
		return E_FAIL;

	DEVICE_CONTEXT->IASetInputLayout(m_Passes[iPassIndex].pInputLayout.Get());

	if (FAILED(m_Passes[iPassIndex].pPass->Apply(0, DEVICE_CONTEXT)))
		return E_FAIL;

	return S_OK;
}

void CShader::Release()
{
}

HRESULT CShader::SetUp_Shader(const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElement)
{
	_uint		iHLSLFlag = 0;

#ifdef _DEBUG
	iHLSLFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_OPTIMIZATION_LEVEL0;
#else
	iHLSLFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif

	if (FAILED(D3DX11CompileEffectFromFile(pShaderFilePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		iHLSLFlag, 0, DEVICE, m_pEffect.GetAddressOf(), /*&pError*/nullptr)))
		return E_FAIL;

	ID3DX11EffectTechnique* pTechnique = m_pEffect->GetTechniqueByIndex(0);

	if (nullptr == pTechnique)
		return E_FAIL;

	D3DX11_TECHNIQUE_DESC		TechniqueDesc;
	ZeroMemory(&TechniqueDesc, sizeof(D3DX11_TECHNIQUE_DESC));

	if (FAILED(pTechnique->GetDesc(&TechniqueDesc)))
		return E_FAIL;

	_uint		iNumPasses = TechniqueDesc.Passes;

	m_Passes.reserve(iNumPasses);

	for (_uint i = 0; i < iNumPasses; ++i)
	{
		PASSDESC			PassDesc;
		ZeroMemory(&PassDesc, sizeof(PASSDESC));

		PassDesc.pPass = pTechnique->GetPassByIndex(i);
		if (nullptr == PassDesc.pPass)
			return E_FAIL;

		D3DX11_PASS_DESC		Pass;
		ZeroMemory(&Pass, sizeof(D3DX11_PASS_DESC));

		PassDesc.pPass->GetDesc(&Pass);

		if (FAILED(DEVICE->CreateInputLayout(pElements, iNumElement, Pass.pIAInputSignature, Pass.IAInputSignatureSize, &PassDesc.pInputLayout)))
			return E_FAIL;

		m_Passes.push_back(PassDesc);
	}

	return S_OK;
}
