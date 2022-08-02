#include "..\Public\Input_Device.h"

IMPLEMENT_SINGLETON(CInput_Device)

CInput_Device::CInput_Device()
{
}


CInput_Device::~CInput_Device()
{
	Release();
}

HRESULT CInput_Device::Initialize(HINSTANCE hInst, HWND hWnd)
{
	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, nullptr)))
		return E_FAIL;

	if(FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	if (FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse)))
		return E_FAIL;
	if (FAILED(m_pMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
		return E_FAIL;
	if (FAILED(m_pMouse->Acquire()))
		return E_FAIL;

	return S_OK;
}

HRESULT CInput_Device::SetUp_DeviceStates()
{
	if (FAILED(m_pMouse->GetDeviceState(sizeof(m_MouseState), &m_MouseState)))
		return E_FAIL;

	return S_OK;
}

void CInput_Device::Release()
{
	Safe_Release(m_pMouse);
	Safe_Release(m_pInputSDK);
}
