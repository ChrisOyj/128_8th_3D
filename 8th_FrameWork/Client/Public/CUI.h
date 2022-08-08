#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
END

BEGIN(Client)

class CUI 
	: public CGameObject
{
	DECLARE_PROTOTYPE(CUI);

protected:
	CUI();
	virtual ~CUI();

public:
	static CUI* Create(const _uint& iID, const _float4&	vStartPosition);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	HRESULT	SetUp_Components(const _float4& vStartPosition);
};

END