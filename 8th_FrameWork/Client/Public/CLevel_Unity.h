#pragma once
#include "Level.h"
#include "Client_Defines.h"

class CLevel_Unity final
	: public CLevel
{
private:
	CLevel_Unity();
	virtual ~CLevel_Unity();

public:
	static CLevel_Unity* Create();

public:
	// CLevel을(를) 통해 상속됨
	virtual HRESULT Enter() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual HRESULT Render() override;
	virtual HRESULT Exit() override;

private:



	

};

