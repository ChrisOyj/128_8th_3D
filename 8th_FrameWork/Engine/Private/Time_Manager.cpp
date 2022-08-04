#include "..\Public\Time_Manager.h"

#include "GameInstance.h"
#include "Key_Manager.h"

IMPLEMENT_SINGLETON(CTime_Manager)


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

_double CTime_Manager::Get_DT()
{
	// 진짜 DT가 FPSLimit (예 : 60분의 1)보다 작으면
	// 프레임이 더 잘나오고 있는거다.
	// 하지만 제한을 걸어야 하므로 1/60 을 리턴.
	//return (m_dDT < m_dFPSLimitTime) ? m_dFPSLimitTime : m_dDT;

	return m_dDT;
}


void CTime_Manager::Tick()
{
	m_bFPSUpdate = false;

	QueryPerformanceCounter(&m_llCurCount);

	m_dDT = (double)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (double)m_llFrequency.QuadPart;
	m_llPrevCount = m_llCurCount;
	

	m_dFPSLimitAcc += m_dDT;
	Render();

	if (m_dFPSLimitAcc > m_dFPSLimitTime)
	{
		m_dFPSLimitAcc = 0.;
		m_bFPSUpdate = true;
	}
}
void CTime_Manager::Render()
{
	if (m_dFPSLimitAcc > m_dFPSLimitTime)
		++m_iCallCount;

	m_dAcc += m_dDT;

	if (m_dFPSUpdateTime <= m_dAcc) {

		m_iFPS = m_iCallCount;
		m_dAcc = 0.;
		m_iCallCount = 0;

		wchar_t szBuffer[255] = {};
		swprintf_s(szBuffer, L"Naruto Shinobi Striker (오용준 모작)                                        FPS : %d,  DT : %f", m_iFPS, m_dDT);
		SetWindowText(m_hWnd, szBuffer);

	}
}
