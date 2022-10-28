#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
END

BEGIN(Client)

class CDefault_UI;

class CUI_Intro
	: public CGameObject
{
	DECLARE_PROTOTYPE(CUI_Intro);

public:
	enum UI_TYPE {
		INTRO_MAINTEXT, INTRO_MISSIONBASE, INTRO_MISSIONTEXT, INTRO_CLEAR_MAINBG, INTRO_CLEAR_MAINTEXT,
		INTRO_INFO, INTRO_MESSAGE, INTRO_NAME, INTRO_TITLE,
		DIALOG_BASE, DIAGLOG_NAMEBASE, DIALOG_NAMETEXT, DIALOG_TEXT,
		UI_END
	};


private:
	CUI_Intro();
	virtual ~CUI_Intro();


public:
	static CUI_Intro* Create();

public:
	void	Set_ShaderResource(CShader* pShader, const char* pConstantName);

public:
	void	On_CinemaMessage(_uint iIndex);
	void	On_CinemaMessageOff();
	void	On_CinemaIntro(_uint iCinemaIntroIndex);
	void	On_CinemaIntroTurnOff();
	void	On_StartIntro(_uint iIndex);
	void	On_Mission(_uint iTextIndex);
	void	On_ClearIntro();
	void	On_Clear();
	void	On_Dialog(_uint iIndex);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;

private:
	//base, 아이콘들
	CDefault_UI* m_arrUI[UI_END];
	_bool		m_bClearUITime = false;
	_bool		m_bBGM = false;
	_float		m_fTimeAcc = 0.f;

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;

	virtual void OnEnable() override;
	virtual void OnDisable() override;

};
END

