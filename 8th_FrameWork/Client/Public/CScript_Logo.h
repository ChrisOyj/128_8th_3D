#pragma once
#include "CScriptable.h"
#include "Client_Defines.h"

#define	SCRIPT_FREECAM_PROTOTYPE_ID	100010

BEGIN(Client)

class CScript_Logo final
	: public CScriptable
{
	DECLARE_PROTOTYPE(CScript_Logo);

private:
	CScript_Logo(_uint iGroupIdx);
	virtual ~CScript_Logo();

public:
	static CScript_Logo* Create(_uint iGroupIdx, LEVEL_TYPE_CLIENT eNextLevelType);



public:
	// CScriptable을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual void Release() override;

public:
	virtual void		OnDisable() override;

private:
	LEVEL_TYPE_CLIENT	m_eNextLevelType = LEVEL_END;
};

END