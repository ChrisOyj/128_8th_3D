#pragma once
#include "CCustom_UI.h"
#include "Client_Defines.h"

BEGIN(Engine)
END

BEGIN(Client)

class CLoading_Turn final
	: public CCustom_UI
{
	DECLARE_PROTOTYPE(CLoading_Turn);

protected:
	CLoading_Turn();
	virtual ~CLoading_Turn();

public:
	static CLoading_Turn* Create(const _float4& vStartPosition = _float4(g_iWinCX * 0.5f, g_iWinCY * 0.5f, 0.f, 1.f));

private:
	HRESULT	SetUp_Components(const _float4& vStartPosition);
};

END