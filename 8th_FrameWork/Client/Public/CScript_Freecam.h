#pragma once
#include "CScriptable.h"
#include "Client_Defines.h"

BEGIN(Client)

class CScript_Freecam final
	: public CScriptable
{
	DECLARE_PROTOTYPE(CScript_Freecam);

private:
	CScript_Freecam();
	virtual ~CScript_Freecam();

public:
	// CScriptable을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual void Release() override;
};

END