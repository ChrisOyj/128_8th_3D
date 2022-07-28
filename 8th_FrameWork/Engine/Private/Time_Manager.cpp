#include "..\Public\Time_Manager.h"

#include "GameInstance.h"
#include "Key_Manager.h"

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

	
}
void CTime_Manager::Render()
{
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
