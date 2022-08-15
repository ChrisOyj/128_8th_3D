#pragma once
#include "CCustom_UI.h"
#include "Client_Defines.h"

BEGIN(Engine)
END

BEGIN(Client)

class CLoading_BG final
	: public CCustom_UI
{
	DECLARE_PROTOTYPE(CLoading_BG);

protected:
	CLoading_BG();
	virtual ~CLoading_BG();

public:
	static CLoading_BG* Create(const _float4& vStartPosition = _float4(g_iWinCX * 0.5f, g_iWinCY * 0.5f, 0.f, 1.f));

private:
	HRESULT	SetUp_Components(const _float4& vStartPosition);
};

END