#pragma once
#include "CUI.h"

BEGIN(Client)

class CSimple_Image final
	: public CUI
{
	DECLARE_PROTOTYPE(CSimple_Image);

	enum SIMPLE_TEXTURE	{SIMPLE_LOADING = 119000, SIMPLE_LOADINGBAR, SIMPLE_END};
	enum SIMPLE_FADER {FADER_LOADING = 119100, FADER_END};

private:
	CSimple_Image();
	virtual ~CSimple_Image();

public:
	static CSimple_Image* Create(
		const _float4& vPosition, const _float2& vScale, const _tchar* strFilePath, const _uint& iNumTextures,
		const SIMPLE_TEXTURE& eSimpleTextureID, const FADEDESC& tFadeDesc, const SIMPLE_FADER& eSimpleFaderID
	);

public:
	void	Set_TextureIdx(const _uint& iIdx);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;

private:
	HRESULT	SetUp_Components(const _float4& vPosition, const _float2& vScale, const _tchar* strFilePath, const _uint& iNumTextures,
		const SIMPLE_TEXTURE& eSimpleTextureID, const FADEDESC& tFadeDesc, const SIMPLE_FADER& eSimpleFaderID);

};

END