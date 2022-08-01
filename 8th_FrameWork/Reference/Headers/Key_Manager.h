#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CKey_Manager
{
	DECLARE_SINGLETON(CKey_Manager);

private:
	CKey_Manager();
	~CKey_Manager();

public:
	KEY_STATE				Get_KeyState(KEY _key);

public:
	HRESULT		Initialize(HWND hWnd);
	void		Tick();

private:
	struct tKeyInfo
	{
		KEY_STATE eState;
		bool bPrevPush;
	};
	HWND				m_hWnd = 0;
	vector<tKeyInfo>	m_vecKey;

};

END