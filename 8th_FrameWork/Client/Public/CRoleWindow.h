#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Client)

class CDefault_UI;

struct PHASE_DESC
{
	_uint	iNumButtons = 0;
	_uint	iMainTextIndex = 0;
	_uint	iBGInfoTextIndex = 0;
	_uint	iTitleTextIndex = 0;
	vector<_uint>	vecButtonTextIndex;

};

class CRoleWindow abstract
	: public CGameObject
{
public:
	enum PHASE{PHASE_DEFAULT, PHASE_END = 32};

protected:
	CRoleWindow();
	virtual ~CRoleWindow();

public:
	void	Set_CurPhase(_uint iCurPhase);
	virtual void	On_FadeOut(_uint iPhase);

public:
	virtual HRESULT Initialize_Prototype() PURE;
	virtual HRESULT Initialize() override;

protected:
	vector<CDefault_UI*>	m_vecAllUIs;
	vector<CDefault_UI*>	m_vecUIs;
	vector<CDefault_UI*>	m_vecButtons;
	vector<CDefault_UI*>	m_vecButtonTexts;
	_uint					m_iCurPhase = 0;
	_uint					m_iNextPhase = 0;
	PHASE_DESC				m_arrPhaseDesc[PHASE_END];

	_int	m_iCurSelection = 0;

	_uint	m_iMainTextIndex = 0;
	_uint	m_iBGInfoTextIndex = 0;
	_uint	m_iTitleTextIndex = 0;
	_uint	m_iFinishButtonIndex = 0;

	_bool	m_bInitializeDone = false;

protected:
	virtual void	OnSelect() PURE;
	virtual void	OnSelectionSwitch(_int iSelection);

protected:
	virtual void My_Tick() override;

private:


};
END

