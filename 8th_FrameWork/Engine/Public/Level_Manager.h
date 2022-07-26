#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CLevel;

class CLevel_Manager
{
	DECLARE_SINGLETON(CLevel_Manager);

private:
	CLevel_Manager();
	virtual ~CLevel_Manager();

public:
	HRESULT	Add_Level(CLevel* pLevel);

public:
	void	Tick();
	void	Late_Tick();
	HRESULT	Render();

private:
	vector<CLevel*>			m_arrLevels;
	_uint					m_iCurrentLevelID = 0;

private:
	friend class CEvent_Manager;
	HRESULT Open_Level(const _uint& iLevelID);
	void	Release();
};

END