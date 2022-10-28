#pragma once
#include "CScriptable.h"
#include "Client_Defines.h"

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
	void	Enter_UnityLevel() { m_bUnityLevel = true; }

public:
	// CScriptable을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual void Release() override;

private:
	_bool	m_bTurnOn = true;
	_bool	m_bUnityLevel = false;

private:
	void	Key_Input();
	void	OnMouseMove();
};

END