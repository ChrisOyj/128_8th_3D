#include "stdafx.h"
#include "CBlender.h"

#include "GameInstance.h"
#include "CShader.h"
#include "Texture.h"

#include "GameObject.h"

CBlender::CBlender(_uint iGroupIdx)
	: CComponent(iGroupIdx)
{
}

CBlender::~CBlender()
{
	Release();
}

CBlender* CBlender::Create(const _tchar* FilterTexturePath, const _tchar* BlendTexturePath)
{
	CBlender* pInstance = new CBlender(0);
	if (FAILED(pInstance->SetUp_Texture(FilterTexturePath, BlendTexturePath)))
	{
		Call_MsgBox(L"Failed to SetUp_Texture : CBlender");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CBlender");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CBlender::Set_FilterSRV(CShader* pShader, const char* pConstantName)
{
	pShader->Set_ShaderResourceView(pConstantName, m_pTextureCom->Get_vecTexture()[0].pSRV);
}

void CBlender::Set_BlendSRV(CShader* pShader, const char* pConstantName)
{
	pShader->Set_ShaderResourceView(pConstantName, m_pTextureCom->Get_vecTexture()[1].pSRV);
}

HRESULT CBlender::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBlender::Initialize()
{
	return S_OK;
}

void CBlender::Tick()
{
}

void CBlender::Late_Tick()
{
}

void CBlender::Release()
{
}

void CBlender::Start()
{
	__super::Start();

	GET_COMPONENT_FROM(m_pOwner, CShader)->CallBack_SetRawValues +=
		bind(&CBlender::Set_FilterSRV, this, placeholders::_1, "g_FilterTexture");

	GET_COMPONENT_FROM(m_pOwner, CShader)->CallBack_SetRawValues +=
		bind(&CBlender::Set_BlendSRV, this, placeholders::_1, "g_BlendTexture");

	m_pOwner->Add_Component(m_pTextureCom);
}

HRESULT CBlender::SetUp_Texture(const _tchar* FilterTexturePath, const _tchar* BlendTexturePath)
{
	CTexture* pTextureCom = CTexture::Create(CP_AFTER_TRANSFORM,
		FilterTexturePath, 1);

	if (!pTextureCom)
		return E_FAIL;

	if (FAILED(pTextureCom->Add_Texture(BlendTexturePath)))
		return E_FAIL;

	pTextureCom->Initialize();
	pTextureCom->Set_CurTextureIndex(0);

	m_pTextureCom = pTextureCom;


	return S_OK;
}
