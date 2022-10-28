#include "stdafx.h"
#include "CRoleWindow.h"

#include "GameInstance.h"
#include "CFader.h"
#include "CDefault_UI.h"

#include "Texture.h"

CRoleWindow::CRoleWindow()
{
}

CRoleWindow::~CRoleWindow()
{
	if (!m_bInitializeDone)
	{
		for (auto& elem : m_vecAllUIs)
			SAFE_DELETE(elem);
	}
}

void CRoleWindow::Set_CurPhase(_uint iCurPhase)
{
	if (m_iCurPhase == iCurPhase)
		return;

	// 1. ��� ���̵� �ƿ� ��Ŵ.
	for (auto& elem : m_vecAllUIs)
	{
		if (elem->Is_Valid())
			GET_COMPONENT_FROM(elem, CFader)->Force_KeyInput();
	}


	m_iNextPhase = iCurPhase;
}

void CRoleWindow::On_FadeOut(_uint iPhase)
{
	OnSelectionSwitch(0);
	m_iCurPhase = m_iNextPhase;

	// 2. ��ư ���� ����
	for (_uint i = 0; i < m_arrPhaseDesc[m_iCurPhase].iNumButtons; ++i)
	{
		ENABLE_GAMEOBJECT(m_vecButtons[i]);
		ENABLE_GAMEOBJECT(m_vecButtonTexts[i]);
	}

	for (_uint i = m_arrPhaseDesc[m_iCurPhase].iNumButtons; i < m_vecButtons.size(); ++i)
	{
		DISABLE_GAMEOBJECT(m_vecButtons[i]);
		DISABLE_GAMEOBJECT(m_vecButtonTexts[i]);
	}

	// 3. �ֵ� �ؽ�Ʈ �̹��� �ùٸ� �ε��� ã�ư�����
	GET_COMPONENT_FROM(m_vecUIs[m_iMainTextIndex], CTexture)->Set_CurTextureIndex(m_arrPhaseDesc[m_iCurPhase].iMainTextIndex);
	GET_COMPONENT_FROM(m_vecUIs[m_iTitleTextIndex], CTexture)->Set_CurTextureIndex(m_arrPhaseDesc[m_iCurPhase].iTitleTextIndex);
	GET_COMPONENT_FROM(m_vecUIs[m_iBGInfoTextIndex], CTexture)->Set_CurTextureIndex(m_arrPhaseDesc[m_iCurPhase].iBGInfoTextIndex);

	for (_uint i = 0; i < m_arrPhaseDesc[m_iCurPhase].iNumButtons; ++i)
		GET_COMPONENT_FROM(m_vecButtonTexts[i], CTexture)->Set_CurTextureIndex(m_arrPhaseDesc[m_iCurPhase].vecButtonTextIndex[i]);

	// 4. ��ư �� Ŭ�� �� ȿ���� ����
}

HRESULT CRoleWindow::Initialize()
{
	m_bInitializeDone = true;
	for (_uint i = 0; i < m_vecUIs.size(); ++i)
	{
		if (m_vecUIs[i]->Get_FileKey() == L"CustomizingMenu_RoleWindow")
		{
			GET_COMPONENT_FROM(m_vecUIs[i], CFader)->Set_FadeMessage();
			m_vecUIs[i]->CallBack_FadeOutEvent += bind(&CRoleWindow::On_FadeOut, this, m_iCurPhase);
			continue;
		}

		if (m_vecUIs[i]->Get_FileKey() == L"CustomizingMenu_RoleTitleText")
		{
			m_iTitleTextIndex = i;
			continue;
		}

		if (m_vecUIs[i]->Get_FileKey() == L"CustomizingMenu_TitleText")
		{
			m_iMainTextIndex = i;
			continue;
		}

		if (m_vecUIs[i]->Get_FileKey() == L"CustomizingMenu_InfoText")
		{
			m_iBGInfoTextIndex = i;
			continue;
		}

		if (m_vecUIs[i]->Get_FileKey() == L"CustomizingMenu_FinishButton")
		{
			m_iFinishButtonIndex = i;
			continue;
		}
	}

	for (auto& elem : m_vecUIs)
		CREATE_GAMEOBJECT(elem, GROUP_UI);
	for (auto& elem : m_vecButtons)
		CREATE_GAMEOBJECT(elem, GROUP_UI);
	for (auto& elem : m_vecButtonTexts)
		CREATE_GAMEOBJECT(elem, GROUP_UI);

	OnSelectionSwitch(0);


	return S_OK;
}

void CRoleWindow::My_Tick()
{
	if (KEY(W, TAP))
	{
		OnSelectionSwitch(m_iCurSelection - 1);
		GAMEINSTANCE->Play_Sound(L"Select", (CHANNEL_GROUP)CHANNEL_UI);

	}
	else if (KEY(S, TAP))
	{
		OnSelectionSwitch(m_iCurSelection + 1);
		GAMEINSTANCE->Play_Sound(L"Select", (CHANNEL_GROUP)CHANNEL_UI);

	}
	else if (KEY(ESC, TAP))
	{
		Set_CurPhase(0);
		GAMEINSTANCE->Play_Sound(L"Select_Back", (CHANNEL_GROUP)CHANNEL_UI);

	}
	else if (KEY(SPACE, TAP))
	{
		OnSelect();
	}
}

void CRoleWindow::OnSelectionSwitch(_int iSelection)
{
	if (iSelection < 0 || iSelection > m_arrPhaseDesc[m_iCurPhase].iNumButtons)
		return;

	if (m_iCurSelection == m_arrPhaseDesc[m_iCurPhase].iNumButtons)
		GET_COMPONENT_FROM(m_vecUIs[m_iFinishButtonIndex], CTexture)->Set_CurTextureIndex(0);
	else
	{
		for (auto& elem : m_vecButtons)
			GET_COMPONENT_FROM(elem, CTexture)->Set_CurTextureIndex(0);
	}

	m_iCurSelection = iSelection;

	if (m_iCurSelection == m_arrPhaseDesc[m_iCurPhase].iNumButtons)
		GET_COMPONENT_FROM(m_vecUIs[m_iFinishButtonIndex], CTexture)->Set_CurTextureIndex(1);
	else
		GET_COMPONENT_FROM(m_vecButtons[m_iCurSelection], CTexture)->Set_CurTextureIndex(1);



}


