#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Loading final 
	: public CLevel
{
private:
	CLevel_Loading();
	virtual ~CLevel_Loading() = default;

public:
	static CLevel_Loading* Create();

public:
	void	Set_NextLevel(LEVEL_TYPE_CLIENT	eLevelID) { m_eNextLevel = eLevelID; }

public:
	// CLevel을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual HRESULT Enter() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual HRESULT Render();
	virtual HRESULT Exit() override;

private:
	LEVEL_TYPE_CLIENT			m_eNextLevel = LEVEL_END;
	

};

END