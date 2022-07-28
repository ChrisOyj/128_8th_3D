#include "..\Public\Texture.h"
#include "GameInstance.h"


CTexture::CTexture(CGameObject* pOwner)
	: CComponent(pOwner)
{
}

CTexture::CTexture(const CTexture& _origin)
	: CComponent(_origin)
{
}

CTexture::~CTexture()
{
	
}
CTexture* CTexture::Create(CGameObject* pOwner, const _tchar* pTextureFilePath, const _uint& iNumTextures)
{
	return nullptr;
}
void CTexture::Tick()
{
}

void CTexture::Late_Tick()
{
}

HRESULT CTexture::Initialize_Prototype()
{
	return E_NOTIMPL;
}

HRESULT CTexture::Initialize()
{
	return E_NOTIMPL;
}

void CTexture::OnEnable()
{
}

void CTexture::OnDisable()
{
}

void CTexture::Release()
{
}
