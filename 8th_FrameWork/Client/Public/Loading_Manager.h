#pragma once
#include "Client_Defines.h"

BEGIN(Engine)
	class CLevel;
class CShader;
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
	bool					IsFinished() { return m_bFinish; }
	CLevel* Get_Level(LEVEL_TYPE_CLIENT eType) { return m_arrLevels[eType]; }

public:
	HRESULT					Initialize();

public:
	HRESULT					Reserve_Load_Level(LEVEL_TYPE_CLIENT	eLevelID);
	HRESULT					Load_Level();
	HRESULT					Start_Loading();
	void					Open_NextLevel();

public:
	void	Set_ShaderResource(CShader* pShader, const char* pConstantName);


private:
	CLevel*					m_arrLevels[LEVEL_END];
	_bool					m_bReservation = false;
	LEVEL_TYPE_CLIENT		m_eLoadID = LEVEL_END;



	HANDLE					m_hThread;
	CRITICAL_SECTION		m_CriticalSection;
	_bool					m_bFinish = false;
	_tchar					m_szLoadingText[MAX_PATH] = TEXT("");

private:
	void		Release();
	void		Finish_LoadingThread();



};

END