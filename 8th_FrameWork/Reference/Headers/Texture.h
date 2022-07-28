#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture
	: public CComponent
{
	DECLARE_PROTOTYPE(CTexture);

private:
	CTexture(CGameObject* pOwner);
	CTexture(const CTexture& _origin);
	virtual ~CTexture();

public:
	static CTexture* Create(CGameObject* pOwner, const _tchar* pTextureFilePath, const _uint& iNumTextures);

public:
	virtual void Tick() override;
	virtual void Late_Tick() override;

private:


private:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void Release() override;

};
END
