#pragma once
#include "CCustom_UI.h"
#include "Client_Defines.h"

BEGIN(Engine)
END

BEGIN(Client)

class CDarkScreen final
	: public CCustom_UI
{
	DECLARE_PROTOTYPE(CDarkScreen);

protected:
	CDarkScreen();
	virtual ~CDarkScreen();

public:
	static CDarkScreen* Create(const _float4& vStartPosition = _float4(g_iWinCX * 0.5f, g_iWinCY * 0.5f, 0.f, 1.f),
		const _float4& vScale = _float4(_float(g_iWinCX), _float(g_iWinCY), 1.f, 1.f));

public:
	virtual HRESULT Initialize() override;


private:
	HRESULT	SetUp_Components(const _float4& vStartPosition, const _float4& vScale);
};

END