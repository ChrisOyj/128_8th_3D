#pragma once
#include "CRoleWindow.h"

BEGIN(Client)

class CRoleWindow_Customizing
	: public CRoleWindow
{
public:
	enum PHASE { PHASE_DEFAULT, PHASE_HEAD, PHASE_EYE, PHASE_FACE, PHASE_OVERALL,
		PHASE_BODYUPPER, PHASE_BODYLOWER, PHASE_ACCESSORY, PHASE_END};

	DECLARE_PROTOTYPE(CRoleWindow_Customizing);

private:
	CRoleWindow_Customizing();
	virtual ~CRoleWindow_Customizing();

public:
	static CRoleWindow_Customizing* Create(class CCustomizingPreview* pPreview);

public:
	// CRoleWindow을(를) 통해 상속됨
	virtual void OnSelect() override;
	virtual void	On_FadeOut(_uint iPhase);

	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Start() override;

private:
	class CCustomizingPreview* m_pPreview = nullptr;
	_bool		m_bFinish = false;

private:
	virtual void	OnSelectionSwitch(_int iSelection) override;

private:
	void	Bake_PhaseDesc(PHASE ePhase, _uint iNumButtons, _uint iMainTextIndex, _uint iTitleTextIndex, _uint iBGInfoTextIndex
		, _uint iButtonTextIndex);

private:
	void	Set_Head(_int iSelection);
	void	Set_Eye(_int iSelection);
	void	Set_Face(_int iSelection);
	void	Set_Overall(_int iSelection);
	void	Set_BodyUpper(_int iSelection);
	void	Set_BodyLower(_int iSelection);
	void	Set_Accessorry(_int iSelection);

private:
	virtual void My_Tick() override;
};

END
