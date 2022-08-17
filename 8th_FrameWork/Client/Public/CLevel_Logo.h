#pragma once
#include "Level.h"
#include "Client_Defines.h"

BEGIN(Client)

class CLevel_Logo final
	: public CLevel
{
	DECLARE_LEVEL(CLevel_Logo);

private:
	CLevel_Logo();
	virtual ~CLevel_Logo();

public:
	// CLevel��(��) ���� ��ӵ�
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

