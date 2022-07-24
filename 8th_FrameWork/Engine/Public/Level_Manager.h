#pragma once

#include "Engine_Defines.h"
/* 현재 게임내에 보여줘야할 레벨을 보관한다. */
/* 레벨교체시에 기존 레벨을 삭제한다. */
/* 기존레벨에서 사용했던 객체와 자원(리소스등)을 정리한다. */

BEGIN(Engine)

class CLevel_Manager final
	: public CManager
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
	class CLevel*			m_pCurrentLevel = nullptr;
	_uint					m_iCurrentLevelID = 0;

private:
	friend class CEvent_Manager;
	HRESULT Open_Level(_uint iLevelID, class CLevel* pLevel);
	void	Release();
};

END