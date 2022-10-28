#include "..\Public\CLight.h"
#include "GameInstance.h"
#include "CShader.h"
#include "CMesh_Rect.h"

CLight::CLight()
{
}

CLight::~CLight()
{
	Release();
}

HRESULT CLight::Initialize(const LIGHTDESC& LightDesc)
{
	m_LightDesc = LightDesc;

	return S_OK;
}

HRESULT CLight::Render(CShader* pShader, CMesh_Rect* pVIBuffer)
{
	if (!GAMEINSTANCE->isIn_Frustum_InWorldSpace(XMLoadFloat4(&m_LightDesc.vPosition), m_LightDesc.fRange))
	{
		return S_OK;
	}


	_uint			iPassIndex = 0;

	/*ºû Á¤º¸¸¦ ¼ÎÀÌ´õ¿¡ ´øÁø´Ù. */
	if (LIGHTDESC::TYPE_DIRECTIONAL == m_LightDesc.eType)
	{
		iPassIndex = 1;

		if (FAILED(pShader->Set_RawValue("g_vLightDir", &m_LightDesc.vDirection, sizeof(_float4))))
			return E_FAIL;
	}

	else
	{
		iPassIndex = 2;

		if (FAILED(pShader->Set_RawValue("g_vLightPos", &m_LightDesc.vPosition, sizeof(_float4))))
			return E_FAIL;

		if (FAILED(pShader->Set_RawValue("g_fRange", &m_LightDesc.fRange, sizeof(_float))))
			return E_FAIL;
	}

	if (FAILED(pShader->Set_RawValue("g_vLightDiffuse", &m_LightDesc.vDiffuse, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(pShader->Set_RawValue("g_vLightAmbient", &m_LightDesc.vAmbient, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(pShader->Set_RawValue("g_vLightSpecular", &m_LightDesc.vSpecular, sizeof(_float4))))
		return E_FAIL;

	pShader->Begin(iPassIndex);

	/* ¼ÎÀÌµå Å¸°Ù¿¡ ±×¸°´Ù.  */
	pVIBuffer->Render();

	return S_OK;
}

CLight* CLight::Create(const LIGHTDESC& LightDesc)
{
	CLight* pInstance = new CLight();

	if (FAILED(pInstance->Initialize(LightDesc)))
	{
		Call_MsgBox(L"Failed to Created : CLight");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CLight::Release()
{
}
