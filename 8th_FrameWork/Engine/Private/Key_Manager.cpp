#include "..\Public\Key_Manager.h"

_uint g_Arr[(_uint)KEY::LAST] = {
	VK_LEFT,//LEFT,
	VK_RIGHT,//RIGHT,
	VK_UP,//UP,
	VK_DOWN,//DOWN,
	'1','2','3','4','5','6','7','8','9','0',
	VK_F1,VK_F2,VK_F3,VK_F4,VK_F5,VK_F6,VK_F7,VK_F8,VK_F9,
	'Q','W','E','R','T','Y','U','I','O','P',
	'A','S','D','F','G','H','J','K','L',
	'Z','X','C','V','B','N','M',
	VK_MENU,//ALT,
	VK_LSHIFT,//LSHIFT,
	VK_SPACE,//SPACE,
	VK_CONTROL,//CTRL,
	VK_RETURN,//ENTER,
	VK_ESCAPE,//ESC,
	VK_TAB,
	VK_LBUTTON,
	VK_RBUTTON,
	VK_BACK,//BACKSPACE
	VK_DELETE,
	//LAST
};

IMPLEMENT_SINGLETON(CKey_Manager)


CKey_Manager::CKey_Manager()
{
}


CKey_Manager::~CKey_Manager()
{
}

KEY_STATE CKey_Manager::Get_KeyState(KEY _key)
{
	return m_vecKey[(int)_key].eState;
}

HRESULT CKey_Manager::Initialize(HWND hWnd)
{
	m_hWnd = hWnd;
	m_vecKey.reserve((_uint)KEY::LAST);

	for (_uint i = 0; i < (_uint)KEY::LAST; ++i)
		m_vecKey.push_back(tKeyInfo{ KEY_STATE::NONE, false });

	return S_OK;
}

void CKey_Manager::Tick()
{
	if (m_hWnd == GetFocus())
	{
		for (int i = 0; i < (int)KEY::LAST; ++i)
		{
			if (GetAsyncKeyState(g_Arr[i]) & 0x8000) {
				if (m_vecKey[i].bPrevPush)
					m_vecKey[i].eState = KEY_STATE::HOLD;
				else
					m_vecKey[i].eState = KEY_STATE::TAP;

				m_vecKey[i].bPrevPush = true; 
			}
			else
			{ // 키가 안눌려있다
				if (m_vecKey[i].bPrevPush)
					m_vecKey[i].eState = KEY_STATE::AWAY;
				else
					m_vecKey[i].eState = KEY_STATE::NONE;

				m_vecKey[i].bPrevPush = false;
			}
		}
	}
	else
	{
		for (int i = 0; i < (int)KEY::LAST; ++i)
		{
			m_vecKey[i].bPrevPush = false;

			switch (m_vecKey[i].eState) {
			case KEY_STATE::TAP:
			case KEY_STATE::HOLD:
				m_vecKey[i].eState = KEY_STATE::AWAY;
				break;

			default:
				m_vecKey[i].eState = KEY_STATE::NONE;
				break;
			}
		}
	}
}
