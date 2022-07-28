#pragma once
#include "Engine_Defines.h"

/* */

BEGIN(Engine)

class CTime_Manager
{
	DECLARE_SINGLETON(CTime_Manager);

private:
	CTime_Manager();
	~CTime_Manager();

public:
	_double	Get_DT() { return m_dDT; };
	void	Set_FPSUpdateTime(_double dTime) { m_dFPSUpdateTime = dTime; }

public:
	HRESULT	Initialize(HWND hWnd);
	void	Tick();
	void	Render();

private:
	HWND			m_hWnd = 0;
	LARGE_INTEGER	m_llCurCount = {};
	LARGE_INTEGER	m_llPrevCount = {};
	LARGE_INTEGER	m_llFrequency = {};

	_double			m_dFPSUpdateTime = 1.;
	_double			m_dDT = 0.;
	_double			m_dAcc = 0.; 
	_uint			m_iCallCount = 0;
	_uint			m_iFPS = 0;
};

}