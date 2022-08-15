#pragma once
#include "Level.h"
#include "Client_Defines.h"

BEGIN(Client)

class CLevel_Logo final
	: public CLevel
{
private:
	CLevel_Logo();
	virtual ~CLevel_Logo();

public:
	static CLevel_Logo* Create();

public:
	// CLevel을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual HRESULT SetUp_Prototypes() override;
	virtual HRESULT Enter() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual HRESULT Render() override;
	virtual HRESULT Exit() override;

private:

};

END

