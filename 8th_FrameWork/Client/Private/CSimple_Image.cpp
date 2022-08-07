#include "stdafx.h"
#include "CSimple_Image.h"

#include "GameInstance.h"

#include "Texture.h"
#include "CShader.h"
#include "Renderer.h"
#include "Transform.h"
#include "Functor.h"
#include "CFader.h"

#include "CComponent_Factory.h"
#include "CPrototype_Factory.h"

CSimple_Image::~CSimple_Image()
{
}

CSimple_Image::CSimple_Image()
{
}

CSimple_Image* CSimple_Image::Create(const _float4& vPosition, const _float2& vScale, const _tchar* strFilePath,
	const _uint& iNumTextures, const SIMPLE_TEXTURE& eSimpleTextureID, const FADEDESC& tFadeDesc, const SIMPLE_FADER& eSimpleFaderID)
{
	CSimple_Image* pInstance = new CSimple_Image();

	_float4	vRealPos = CFunctor::OrthoToRealPosition(vPosition);

	if (FAILED(pInstance->SetUp_Components(vRealPos, vScale, strFilePath, iNumTextures, eSimpleTextureID, tFadeDesc, eSimpleFaderID)))
	{
		Call_MsgBox(L"Failed to SetUp_Components : CSimple_Image");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to SetUp_Components : CSimple_Image");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CSimple_Image::Set_TextureIdx(const _uint& iIdx)
{
	Get_Component<CTexture>()[0]->Set_CurTextureIndex(iIdx);
}

HRESULT CSimple_Image::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSimple_Image::Initialize()
{
	return S_OK;
}

HRESULT CSimple_Image::SetUp_Components(const _float4& vPosition, const _float2& vScale, const _tchar* strFilePath,
	const _uint& iNumTextures, const SIMPLE_TEXTURE& eSimpleTextureID, const FADEDESC& tFadeDesc, const SIMPLE_FADER& eSimpleFaderID)
{
	m_pTransform->Set_World(WORLD_POS, vPosition);
	m_pTransform->Set_Scale(_float4(vScale.x, vScale.y, 0.f, 1.f));

	/* Texture */

	if (!CFunctor::Check_Component_Prototype_Exist(eSimpleTextureID))
	{
		CTexture* pTexture = CTexture::Create(CP_END, strFilePath, iNumTextures);

		if (!pTexture)
			return E_FAIL;

		if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(eSimpleTextureID, pTexture)))
			return E_FAIL;
	}

	Add_Component(CComponent_Factory::Create_FromPrototype(eSimpleTextureID, this));

	/* Fader */
	if (eSimpleFaderID != FADER_END)
	{
		if (!CFunctor::Check_Component_Prototype_Exist(eSimpleFaderID))
		{
			CFader* pFader = CFader::Create(CP_AFTER_TRANSFORM, tFadeDesc);

			if (!pFader)
				return E_FAIL;

			if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(eSimpleFaderID, pFader)))
				return E_FAIL;
		}

		Add_Component(CComponent_Factory::Create_FromPrototype(eSimpleFaderID, this));
	}

	



	Add_Component(CComponent_Factory::Create_FromPrototype(CPrototype_Factory::DEFAULT_SHADER, this));
	Add_Component(CComponent_Factory::Create_FromPrototype(CPrototype_Factory::DEFAULT_MESH_RECT, this));
	Add_Component(CComponent_Factory::Create_FromPrototype(CPrototype_Factory::DEFAULT_UI_RENDERER, this));




	return S_OK;
}
