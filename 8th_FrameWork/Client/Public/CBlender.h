#pragma once
#include "Component.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CTexture;
class CShader;
END

BEGIN(Client)
class CBlender final
	: public CComponent
{
	DECLARE_PROTOTYPE(CBlender)
private:
	CBlender(_uint iGroupIdx);
	virtual ~CBlender();

public:
	static CBlender* Create(const _tchar* FilterTexturePath, const _tchar* BlendTexturePath);

public:
	void Set_FilterSRV(CShader* pShader, const char* pConstantName);
	void Set_BlendSRV(CShader* pShader, const char* pConstantName);


public:
	// CComponent을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual void Release() override;

	virtual void	Start() override;


private:
	CTexture* m_pTextureCom = nullptr;


private:
	HRESULT	SetUp_Texture(const _tchar* FilterTexturePath, const _tchar* BlendTexturePath);
};

END