#pragma once
#include "CCustom_UI.h"
#include "Client_Defines.h"

BEGIN(Engine)
END

BEGIN(Client)

class CLoading_Bar final
	: public CCustom_UI
{
	DECLARE_PROTOTYPE(CLoading_Bar);

protected:
	CLoading_Bar();
	virtual ~CLoading_Bar();

public:
	static CLoading_Bar* Create(const _float4& vStartPosition = _float4(g_iWinCX * 0.5f, g_iWinCY * 0.5f, 0.f, 1.f));

public:
	HRESULT	Initialize_Prototype() override;

private:
	HRESULT	SetUp_Components(const _float4& vStartPosition);
};

END