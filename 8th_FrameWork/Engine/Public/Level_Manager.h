#pragma once

#include "Engine_Defines.h"
/* ���� ���ӳ��� ��������� ������ �����Ѵ�. */
/* ������ü�ÿ� ���� ������ �����Ѵ�. */
/* ������������ ����ߴ� ��ü�� �ڿ�(���ҽ���)�� �����Ѵ�. */

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