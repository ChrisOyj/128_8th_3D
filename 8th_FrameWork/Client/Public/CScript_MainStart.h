#pragma once
#include "CScriptable.h"
#include "Client_Defines.h"

BEGIN(Client)

class CScript_MainStart final
	: public CScriptable
{
	DECLARE_PROTOTYPE(CScript_MainStart);

private:
	CScript_MainStart(_uint iGroupIdx);
	virtual ~CScript_MainStart();

public:
	static CScript_MainStart* Create(_uint iGroupIdx, vector<CGameObject*>&	vecUIs);

public:
	// CScriptable을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual void Release() override;
	virtual void OnDisable() override;

private:
	_bool		m_bOnce = false;
	CGameObject* m_pEffects[2];
	vector<CGameObject*>	m_vecUIs;
};

END