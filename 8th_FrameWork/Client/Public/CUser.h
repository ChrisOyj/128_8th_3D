#pragma once
#include "Client_Defines.h"

BEGIN(Engine)
END

BEGIN(Client)
class CUser
{
	DECLARE_SINGLETON(CUser);

private:
	CUser();
	~CUser();

public:
	HRESULT	Initialize();
	void	Tick();

public:
	void	Fix_CursorPosToCenter();
	void	KeyInput_FPSSetter();

private:
	POINT	m_ptCenter;
};
END
