#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CLevel;

class CLoad_Manager
{
	DECLARE_SINGLETON(CLoad_Manager);

private:
	CLoad_Manager();
	virtual ~CLoad_Manager();

public:
	const CRITICAL_SECTION&	Get_CS() { return m_CriticalSection; }
	bool					IsFinish() { return m_bFinish; }

public:
	HRESULT					Load_Level(CLevel* pLevel);
	HRESULT					Start_Loading();

private:
	CLevel*					m_pLoadLevel = nullptr;

	HANDLE					m_hThread;
	CRITICAL_SECTION		m_CriticalSection;
	_bool					m_bFinish = false;
	_tchar					m_szLoadingText[MAX_PATH] = TEXT("");

private:
	void		Release();

};

END