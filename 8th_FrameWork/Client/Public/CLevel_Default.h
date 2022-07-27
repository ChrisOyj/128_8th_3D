#pragma once
#include "Level.h"
#include "Client_Defines.h"

BEGIN(Client)

class CLevel_Default
	: public CLevel
{
private:
	CLevel_Default();
	virtual ~CLevel_Default();

public:
	static CLevel_Default* Create(const _tchar* strFilePath);

public:
	// CLevel을(를) 통해 상속됨
	virtual void Enter() override;
	virtual void Tick() override;
	virtual HRESULT Render() override;
	virtual void Exit() override;
};

END