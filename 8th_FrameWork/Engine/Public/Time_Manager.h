#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CTime_Manager final
	: public CManager
{
	DECLARE_SINGLETON(CTime_Manager);
private:
	CTime_Manager();
	~CTime_Manager();

public:
	double	Get_DT(bool bReal) { return (bReal) ? m_dDT : m_dDT * m_dTimeSpeed; };

public:
	HRESULT	Initialize(HWND hWnd);
	void	Tick();
	void	Render();

private:
	HWND			m_hWnd = 0;
	LARGE_INTEGER	m_llCurCount = {};
	LARGE_INTEGER	m_llPrevCount = {};
	LARGE_INTEGER	m_llFrequency = {};

	_double			m_dDT = 0.;
	_double			m_dTimeSpeed = 1.;
	_double			m_dAcc = 0.; 
	_uint			m_iCallCount = 0;
	_uint			m_iFPS = 0;
};

}