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
	void	Tick();
	void	Late_Tick();
	HRESULT	Render();

private:
	CLevel*					m_pCurrentLevel = nullptr;

private:
	friend class CEvent_Manager;
	HRESULT Enter_Level(CLevel* pLevel);
	void	Release();
};

END