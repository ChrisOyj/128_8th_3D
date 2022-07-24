#include "..\Public\Time_Manager.h"

#include "GameInstance.h"

CTime_Manager::CTime_Manager()
{
}


CTime_Manager::~CTime_Manager()
{
}

HRESULT CTime_Manager::Initialize(HWND hWnd)
{
	m_hWnd = hWnd;
	QueryPerformanceCounter(&m_llPrevCount);
	QueryPerformanceFrequency(&m_llFrequency);


	return S_OK;
}

void CTime_Manager::Tick()
{
	QueryPerformanceCounter(&m_llCurCount);

	m_dDT = (double)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (double)m_llFrequency.QuadPart;
	m_llPrevCount = m_llCurCount;

//#ifdef _DEBUG
//
//	m_dDT = 1. / 60.;
//
//#endif

	if (KEY(F3, TAP))
	{
		if (m_dTimeSpeed == 1.)
			m_dTimeSpeed = 0.3;
		else
			m_dTimeSpeed = 1.;
	}
	
}
void CTime_Manager::Render()
{
	++m_iCallCount;
	m_dAcc += m_dDT; // DT가 dAcc에 누적되는 중

	if (1 <= m_dAcc) { // 1초가 지난 시점

		m_iFPS = m_iCallCount;
		m_dAcc = 0.;
		m_iCallCount = 0;

		wchar_t szBuffer[255] = {};
		swprintf_s(szBuffer, L"SR                                                            FPS : %d,  DT : %f", m_iFPS, m_dDT);
		SetWindowText(m_hWnd, szBuffer);

	}
}
