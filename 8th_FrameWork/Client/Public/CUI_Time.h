#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Client)

class CDefault_UI;

class CUI_Time
	: public CGameObject
{
	DECLARE_PROTOTYPE(CUI_Time);
	
public:
	enum UI_TYPE {TIME_BASE, TIME_COLON, TIME_MIN, TIME_SECBACK, TIME_SECFRONT , TIME_END};
private:
	CUI_Time();
	virtual ~CUI_Time();


public:
	static CUI_Time* Create();

public:
	void	Reset_Time(_int iMin = 9, _int iSecFront = 5, _int iSecBack = 9);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;

private:
	//base, ½Ã°£µé
	CDefault_UI*	m_arrUI[TIME_END];

	_float			m_fCurTimeAcc = 0.f;

	_int			m_iCurMin = 0;
	_int			m_iCurSecFront = 0;
	_int			m_iCurSecBack = 0;

	_bool			m_bStopTimer = false;

private:
	virtual void My_Tick() override;

	virtual void OnEnable() override;
	virtual void OnDisable() override;

};
END

