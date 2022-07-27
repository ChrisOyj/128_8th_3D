#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)
class CInput_Device
{
	DECLARE_SINGLETON(CInput_Device)
	
private:
	CInput_Device();
	~CInput_Device();

public:
	HRESULT	Initialize(HINSTANCE hInst, HWND hWnd);
	HRESULT SetUp_DeviceStates();

public:
	_long	Get_DIMouseMoveState(MOUSEMOVE eMouseMove)
	{
		return *((_long*)&m_MouseState + eMouseMove);
	}

private:
	LPDIRECTINPUT8			m_pInputSDK = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse = nullptr;
	DIMOUSESTATE			m_MouseState;

private:
	void	Release();
};
END
