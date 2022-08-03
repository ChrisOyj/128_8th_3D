#pragma once
#include "CScriptable.h"
#include "Client_Defines.h"

#define	SCRIPT_FREECAM_PROTOTYPE_ID	100010

BEGIN(Client)

class CScript_Freecam final
	: public CScriptable
{
	DECLARE_PROTOTYPE(CScript_Freecam);

private:
	CScript_Freecam(_uint iGroupIdx);
	virtual ~CScript_Freecam();

public:
	static CScript_Freecam* Create(_uint iGroupIdx);

public:
	// CScriptable을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual void Release() override;
};

END