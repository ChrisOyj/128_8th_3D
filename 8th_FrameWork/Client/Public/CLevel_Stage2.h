#pragma once
#include "Level.h"
#include "Client_Defines.h"

BEGIN(Client)

class CLevel_Stage2 final
	: public CLevel
{
private:
	CLevel_Stage2();
	virtual ~CLevel_Stage2();

public:
	static CLevel_Stage2* Create();

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
	class CTerrain* m_pTerrain = nullptr;


	_float		m_fShadowDelay = 0.f;
	_bool		m_bStaticShadow = false;
	vector<CGameObject*>	m_vecStaticShadowObjects;

};

END

