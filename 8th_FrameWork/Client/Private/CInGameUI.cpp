#include "stdafx.h"
#include "CInGameUI.h"

#include "GameInstance.h"
#include "CFader.h"
#include "CDefault_UI.h"
#include "CUI_Time.h"
#include "CUI_Skill.h"
#include "CUI_HUD.h"
#include "CUI_Combo.h"
#include "CUI_Intro.h"
#include "Texture.h"

CInGameUI::CInGameUI()
{
}

CInGameUI::~CInGameUI()
{
}

CInGameUI* CInGameUI::Create()
{
	CInGameUI* pInstance = new CInGameUI;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CInGameUI");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CInGameUI::Initialize_Prototype()
{
	if (!(m_pUI_Time = CUI_Time::Create()))
		return E_FAIL;


	if (!(m_pUI_Skill = CUI_Skill::Create()))
		return E_FAIL;

	if (!(m_pUI_Combo = CUI_Combo::Create()))
		return E_FAIL;

	if (!(m_pUI_HUD = CUI_HUD::Create()))
		return E_FAIL;

	if (!(m_pUI_Intro = CUI_Intro::Create()))
		return E_FAIL;

	return S_OK;
}

HRESULT CInGameUI::Initialize()
{
	if (FAILED(m_pUI_Time->Initialize()))
		return E_FAIL;

	if (FAILED(m_pUI_Skill->Initialize()))
		return E_FAIL;

	if (FAILED(m_pUI_Combo->Initialize()))
		return E_FAIL;

	if (FAILED(m_pUI_HUD->Initialize()))
		return E_FAIL;

	if (FAILED(m_pUI_Intro->Initialize()))
		return E_FAIL;

	CREATE_STATIC(m_pUI_Skill, HASHCODE(CUI_Skill));
	CREATE_STATIC(m_pUI_Combo, HASHCODE(CUI_Combo));
	CREATE_STATIC(m_pUI_Time, HASHCODE(CUI_Time));
	CREATE_STATIC(m_pUI_HUD, HASHCODE(CUI_HUD));
	CREATE_STATIC(m_pUI_Intro, HASHCODE(CUI_Intro));

	return S_OK;
}

HRESULT CInGameUI::Start()
{
	__super::Start();

	return S_OK;
}

void CInGameUI::Disable_HUD()
{
	DISABLE_GAMEOBJECT(m_pUI_HUD);
	DISABLE_GAMEOBJECT(m_pUI_Skill);
	DISABLE_GAMEOBJECT(m_pUI_Time);
}

void CInGameUI::Enable_HUD()
{
	ENABLE_GAMEOBJECT(m_pUI_HUD);
	ENABLE_GAMEOBJECT(m_pUI_Skill);
	ENABLE_GAMEOBJECT(m_pUI_Time);
}

void CInGameUI::My_Tick()
{
}

void CInGameUI::OnEnable()
{
	__super::OnEnable();
	ENABLE_GAMEOBJECT(m_pUI_Time);
	ENABLE_GAMEOBJECT(m_pUI_HUD);
	ENABLE_GAMEOBJECT(m_pUI_Skill);
	ENABLE_GAMEOBJECT(m_pUI_Combo);
	ENABLE_GAMEOBJECT(m_pUI_Intro);
}

void CInGameUI::OnDisable()
{
	__super::OnDisable();
	DISABLE_GAMEOBJECT(m_pUI_Time);
	DISABLE_GAMEOBJECT(m_pUI_Skill);
	DISABLE_GAMEOBJECT(m_pUI_Combo);
	DISABLE_GAMEOBJECT(m_pUI_HUD);
	DISABLE_GAMEOBJECT(m_pUI_Intro);
}
