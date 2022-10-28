#pragma once
#include "CCustom_UI.h"
#include "Client_Defines.h"

BEGIN(Engine)
END

BEGIN(Client)

class CFadeDark final
	: public CCustom_UI
{
	DECLARE_PROTOTYPE(CFadeDark);

protected:
	CFadeDark();
	virtual ~CFadeDark();

public:
	static CFadeDark* Create(_float fFadeInTime, _float fFadeDelayTime, _float fFadeOutTime);

public:
	virtual HRESULT Initialize() override;

private:
	HRESULT	SetUp_Components(_float fFadeInTime, _float fFadeDelayTime, _float fFadeOutTime);
};

END