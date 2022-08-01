#pragma once
#include "Client_Defines.h"

BEGIN(Engine)
	class CLevel;
END

BEGIN(Client)



class CLoading_Manager
{
	DECLARE_SINGLETON(CLoading_Manager);

private:
	CLoading_Manager();
	virtual ~CLoading_Manager();

public:
	const CRITICAL_SECTION&	Get_CS() { return m_CriticalSection; }
	bool					IsFinish() { return m_bFinish; }

public:
	HRESULT					Initialize();

public:
	HRESULT					Load_Level(LEVEL_TYPE_CLIENT	eLevelID);
	HRESULT					Start_Loading();

private:
	CLevel*					m_arrLevels[LEVEL_END];
	LEVEL_TYPE_CLIENT			m_eLoadID = LEVEL_END;



	HANDLE					m_hThread;
	CRITICAL_SECTION		m_CriticalSection;
	_bool					m_bFinish = false;
	_tchar					m_szLoadingText[MAX_PATH] = TEXT("");

private:
	void		Release();
	void		Finish_LoadingThread();

};

END