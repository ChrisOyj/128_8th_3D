#pragma once
#include "Client_Defines.h"

BEGIN(Client)

class CState;

class CState_Manager
{
	DECLARE_SINGLETON(CState_Manager);

private:
	CState_Manager();
	~CState_Manager();

public:
	CState* Get_State(STATE_TYPE eType)
	{
		return m_arrStates[eType];
	}

public:
	HRESULT	Initialize();

private:
	CState* m_arrStates[STATE_END] = {};

};
END

