#pragma once
#include "CCustom_UI.h"
#include "Client_Defines.h"

BEGIN(Engine)
END

BEGIN(Client)

class CCinemaDark final
	: public CCustom_UI
{
	DECLARE_PROTOTYPE(CCinemaDark);

protected:
	CCinemaDark();
	virtual ~CCinemaDark();

public:
	static CCinemaDark* Create(_bool bUp);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
};

END