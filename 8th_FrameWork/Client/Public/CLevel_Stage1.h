#pragma once
#include "Level.h"
#include "Client_Defines.h"

BEGIN(Client)

class CLevel_Stage1 final
	: public CLevel
{
private:
	CLevel_Stage1();
	virtual ~CLevel_Stage1();

public:
	static CLevel_Stage1* Create();

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
	_float		m_fShadowDelay = 0.f;
	_bool		m_bStaticShadow = false;
	CGameObject* m_pTestObj = nullptr;

	vector<CGameObject*>	m_vecStaticShadowObjects;
};

END

