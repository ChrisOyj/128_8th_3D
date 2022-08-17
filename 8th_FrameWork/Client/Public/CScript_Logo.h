#pragma once
#include "CScriptable.h"
#include "Client_Defines.h"

BEGIN(Client)

class CScript_Logo final
	: public CScriptable
{
	DECLARE_PROTOTYPE(CScript_Logo);

private:
	CScript_Logo(_uint iGroupIdx);
	virtual ~CScript_Logo();

public:
	static CScript_Logo* Create(_uint iGroupIdx, CGameObject* pBG);

public:
	// CScriptable을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual void Release() override;
	virtual void OnDisable() override;

private:
	CGameObject* m_pBG = nullptr;
};

END