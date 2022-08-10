#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CKey_Manager
{
public:
	struct tKeyInfo
	{
		KEY_STATE eState;
		bool bPrevPush;
	};

	DECLARE_SINGLETON(CKey_Manager);

private:
	CKey_Manager();
	~CKey_Manager();

public:
	KEY_STATE				Get_KeyState(KEY _key);
	vector<tKeyInfo>&		Get_KeyList() { return m_vecKey; }

public:
	HRESULT		Initialize(HWND hWnd);
	void		Tick();

private:
	
	HWND				m_hWnd = 0;
	vector<tKeyInfo>	m_vecKey;

};

END