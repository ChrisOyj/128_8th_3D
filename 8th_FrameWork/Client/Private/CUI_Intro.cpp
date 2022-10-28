#include "stdafx.h"
#include "CUI_Intro.h"

#include "GameInstance.h"

#include "CGameObject_Factory.h"
#include "Texture.h"

#include "CDefault_UI.h"

#include "Functor.h"
#include "CShader.h"
#include "CFader.h"

#include "CUnit_Player.h"
#include "CUser.h"
#include "CScript_FollowCam.h"

#include "Transform.h"

#include "CKOLine.h"



CUI_Intro::CUI_Intro()
{
}

CUI_Intro::~CUI_Intro()
{
}

CUI_Intro* CUI_Intro::Create()
{
	CUI_Intro* pInstance = new CUI_Intro;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CUI_Intro");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CUI_Intro::Set_ShaderResource(CShader* pShader, const char* pConstantName)
{
}
void CUI_Intro::On_CinemaMessage(_uint iIndex)
{
	ENABLE_GAMEOBJECT(m_arrUI[INTRO_MESSAGE]);
	GET_COMPONENT_FROM(m_arrUI[INTRO_MESSAGE], CFader)->Re_FadeIn();
	GET_COMPONENT_FROM(m_arrUI[INTRO_MESSAGE], CTexture)->Set_CurTextureIndex(iIndex);
}
void CUI_Intro::On_CinemaMessageOff()
{
	DISABLE_GAMEOBJECT(m_arrUI[INTRO_MESSAGE]);
}
void CUI_Intro::On_CinemaIntro(_uint iCinemaIntroIndex)
{
	ENABLE_GAMEOBJECT(m_arrUI[INTRO_INFO]);
	ENABLE_GAMEOBJECT(m_arrUI[INTRO_TITLE]);
	ENABLE_GAMEOBJECT(m_arrUI[INTRO_NAME]);

	


	GET_COMPONENT_FROM(m_arrUI[INTRO_INFO], CTexture)->Set_CurTextureIndex(iCinemaIntroIndex);
	GET_COMPONENT_FROM(m_arrUI[INTRO_TITLE], CTexture)->Set_CurTextureIndex(iCinemaIntroIndex);
	GET_COMPONENT_FROM(m_arrUI[INTRO_NAME], CTexture)->Set_CurTextureIndex(iCinemaIntroIndex);

	if (iCinemaIntroIndex == 1)
	{
		GET_COMPONENT_FROM(m_arrUI[INTRO_INFO], CFader)->Re_FadeIn();
		GET_COMPONENT_FROM(m_arrUI[INTRO_TITLE], CFader)->Re_FadeIn();
		GET_COMPONENT_FROM(m_arrUI[INTRO_NAME], CFader)->Re_FadeIn();
	}
	else
	{
		GET_COMPONENT_FROM(m_arrUI[INTRO_INFO], CFader)->Force_KeyInput();
		GET_COMPONENT_FROM(m_arrUI[INTRO_TITLE], CFader)->Force_KeyInput();
		GET_COMPONENT_FROM(m_arrUI[INTRO_NAME], CFader)->Force_KeyInput();
	}
	

	GET_COMPONENT_FROM(m_arrUI[INTRO_INFO], CFader)->Get_FadeDesc().eFadeOutType = FADEDESC::FADEOUT_DISABLE;
	GET_COMPONENT_FROM(m_arrUI[INTRO_TITLE], CFader)->Get_FadeDesc().eFadeOutType = FADEDESC::FADEOUT_DISABLE;
	GET_COMPONENT_FROM(m_arrUI[INTRO_NAME], CFader)->Get_FadeDesc().eFadeOutType = FADEDESC::FADEOUT_DISABLE;

}

void CUI_Intro::On_CinemaIntroTurnOff()
{
	GET_COMPONENT_FROM(m_arrUI[INTRO_INFO], CFader)->Re_FadeOut();
	GET_COMPONENT_FROM(m_arrUI[INTRO_TITLE], CFader)->Re_FadeOut();
	GET_COMPONENT_FROM(m_arrUI[INTRO_NAME], CFader)->Re_FadeOut();
	GET_COMPONENT_FROM(m_arrUI[INTRO_MESSAGE], CFader)->Re_FadeOut();
}

void CUI_Intro::On_StartIntro(_uint iIndex)
{
	GAMEINSTANCE->Play_Sound(L"Intro_GameStart", (CHANNEL_GROUP)CHANNEL_UI);

	ENABLE_GAMEOBJECT(m_arrUI[INTRO_MAINTEXT]);
	ENABLE_GAMEOBJECT(m_arrUI[INTRO_MISSIONBASE]);
	ENABLE_GAMEOBJECT(m_arrUI[INTRO_MISSIONTEXT]);
	GET_COMPONENT_FROM(m_arrUI[INTRO_MISSIONTEXT], CTexture)->Set_CurTextureIndex(iIndex);

	if (iIndex == 2)
	{
		GET_COMPONENT_FROM(m_arrUI[INTRO_MAINTEXT], CFader)->Re_FadeIn();
		GET_COMPONENT_FROM(m_arrUI[INTRO_MISSIONBASE], CFader)->Re_FadeIn();
		GET_COMPONENT_FROM(m_arrUI[INTRO_MISSIONTEXT], CFader)->Re_FadeIn();

	}

}

void CUI_Intro::On_Mission(_uint iTextIndex)
{
	GAMEINSTANCE->Play_Sound(L"Intro_MissionAlert", (CHANNEL_GROUP)CHANNEL_UI);

	ENABLE_GAMEOBJECT(m_arrUI[INTRO_MISSIONBASE]);
	ENABLE_GAMEOBJECT(m_arrUI[INTRO_MISSIONTEXT]);
	GET_COMPONENT_FROM(m_arrUI[INTRO_MISSIONTEXT], CTexture)->Set_CurTextureIndex(iTextIndex);

	GET_COMPONENT_FROM(m_arrUI[INTRO_MISSIONBASE], CFader)->Re_FadeIn();
	GET_COMPONENT_FROM(m_arrUI[INTRO_MISSIONTEXT], CFader)->Re_FadeIn();
}

void CUI_Intro::On_ClearIntro()
{
	m_bBGM = false;
	m_bClearUITime = true;
	m_fTimeAcc = 0.f;
	ENABLE_GAMEOBJECT(m_arrUI[INTRO_MISSIONBASE]);
	ENABLE_GAMEOBJECT(m_arrUI[INTRO_MISSIONTEXT]);

	GET_COMPONENT_FROM(m_arrUI[INTRO_MISSIONBASE], CFader)->Re_FadeIn();
	GET_COMPONENT_FROM(m_arrUI[INTRO_MISSIONTEXT], CFader)->Re_FadeIn();

	GET_COMPONENT_FROM(m_arrUI[INTRO_MISSIONTEXT], CTexture)->Set_CurTextureIndex(1);
}

void CUI_Intro::On_Clear()
{
	GAMEINSTANCE->Play_Sound(L"Intro_GameEnd", (CHANNEL_GROUP)CHANNEL_UI);
	GAMEINSTANCE->Play_Sound(L"BGM_Clear", (CHANNEL_GROUP)CHANNEL_UI);

	ENABLE_GAMEOBJECT(m_arrUI[INTRO_CLEAR_MAINBG]);
	ENABLE_GAMEOBJECT(m_arrUI[INTRO_CLEAR_MAINTEXT]);

	GET_COMPONENT_FROM(m_arrUI[INTRO_CLEAR_MAINBG], CFader)->Re_FadeIn();
	GET_COMPONENT_FROM(m_arrUI[INTRO_CLEAR_MAINTEXT], CFader)->Re_FadeIn();
}

void CUI_Intro::On_Dialog(_uint iIndex)
{
	for (_uint i = DIALOG_BASE; i < UI_END; ++i)
	{
		ENABLE_GAMEOBJECT(m_arrUI[i]);
		GET_COMPONENT_FROM(m_arrUI[i], CFader)->Force_KeyInput();
		//GET_COMPONENT_FROM(m_arrUI[i], CFader)->Get_FadeDesc().eFadeOutType = FADEDESC::FADEOUT_DISABLE;

	}

	GET_COMPONENT_FROM(m_arrUI[DIALOG_TEXT], CTexture)->Set_CurTextureIndex(iIndex);
	//GET_COMPONENT_FROM(m_arrUI[DIALOG_TEXT], CFader)->Force_KeyInput();

	//이벤트에 따라 Fade 메세지를 줘야함

	if (iIndex != 1 && iIndex < 5)
	{
		GET_COMPONENT_FROM(m_arrUI[DIALOG_TEXT], CFader)->Set_FadeMessage();
		m_arrUI[DIALOG_TEXT]->CallBack_FadeOutEvent.Clear();
		m_arrUI[DIALOG_TEXT]->CallBack_FadeOutEvent += bind(&CUser::On_Stage2Event, CUser::Get_Instance(), iIndex);
	}
	else
	{
		GET_COMPONENT_FROM(m_arrUI[DIALOG_TEXT], CFader)->No_FadeMessage();
		//GET_COMPONENT_FROM(m_arrUI[DIALOG_TEXT], CTexture)->Set_CurTextureIndex(0);

	}

	if (iIndex == 10)
	{
		GET_COMPONENT_FROM(m_arrUI[DIALOG_TEXT], CFader)->Set_FadeMessage();
		m_arrUI[DIALOG_TEXT]->CallBack_FadeOutEvent.Clear();
		m_arrUI[DIALOG_TEXT]->CallBack_FadeOutEvent += bind(&CUser::On_FinalCutScene, CUser::Get_Instance(), iIndex);
	}

	


}


HRESULT CUI_Intro::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CUI_Intro::Initialize()
{
	vector<CDefault_UI*>	vecUIs = CGameObject_Factory::Create_vecUI_Reverse(L"InGame_Start");

	//이름순으로 숙숟 들어오게
	for (_uint i = 0; i < vecUIs.size(); ++i)
	{
		m_arrUI[i] = vecUIs[i];
		CREATE_STATIC(m_arrUI[i], Convert_ToHash(m_arrUI[i]->Get_FileKey()));
		GET_COMPONENT_FROM(m_arrUI[i], CFader)->Get_FadeDesc().eFadeOutType = FADEDESC::FADEOUT_DISABLE;
		DISABLE_GAMEOBJECT(m_arrUI[i]);
	}

	vecUIs = CGameObject_Factory::Create_vecUI_Reverse(L"InGame_Clear");

	for (_uint i = 3; i < INTRO_INFO; ++i)
	{
		m_arrUI[i] = vecUIs[i-3];
		CREATE_STATIC(m_arrUI[i], Convert_ToHash(m_arrUI[i]->Get_FileKey()));
		GET_COMPONENT_FROM(m_arrUI[i], CFader)->Get_FadeDesc().eFadeOutType = FADEDESC::FADEOUT_DISABLE;
		DISABLE_GAMEOBJECT(m_arrUI[i]);

	}

	vecUIs = CGameObject_Factory::Create_vecUI_Reverse(L"InGame_Intro");

	for (_uint i = INTRO_INFO; i < DIALOG_BASE; ++i)
	{
		m_arrUI[i] = vecUIs[i - 5];
		CREATE_STATIC(m_arrUI[i], Convert_ToHash(m_arrUI[i]->Get_FileKey()));

		GET_COMPONENT_FROM(m_arrUI[i], CFader)->Get_FadeDesc().eFadeOutType = FADEDESC::FADEOUT_DISABLE;
	}

	DISABLE_GAMEOBJECT(m_arrUI[INTRO_MESSAGE]);

	vecUIs = CGameObject_Factory::Create_vecUI_Reverse(L"InGame_Dialog");

	for (_uint i = DIALOG_BASE; i < UI_END; ++i)
	{
		m_arrUI[i] = vecUIs[i - 9];
		CREATE_STATIC(m_arrUI[i], Convert_ToHash(m_arrUI[i]->Get_FileKey()));
		DISABLE_GAMEOBJECT(m_arrUI[i]);

	}
	GET_COMPONENT_FROM(m_arrUI[DIALOG_TEXT], CFader)->Set_FadeMessage();


	return S_OK;
}

void CUI_Intro::OnEnable()
{
	__super::OnEnable();

}

void CUI_Intro::OnDisable()
{
	__super::OnDisable();
}

void CUI_Intro::My_Tick()
{
	if (!m_bClearUITime)
		return;


	m_fTimeAcc += fDT;


	if (!m_bBGM)
	{
		_float fBGMTime = 2.f;
		if (m_fTimeAcc > fBGMTime)
		{
			m_bBGM = true;
			GAMEINSTANCE->Set_ChannelVolume((CHANNEL_GROUP)CHANNEL_BGM, 0.3f);
			GAMEINSTANCE->Stop_Sound((CHANNEL_GROUP)CHANNEL_BGM);
		}
		else
		{
			_float fVolume = 0.3f * ((fBGMTime - m_fTimeAcc) / fBGMTime);
			GAMEINSTANCE->Set_ChannelVolume((CHANNEL_GROUP)CHANNEL_BGM, fVolume);

		}


	}


	//브금 사운드 줄어들기

	if (m_fTimeAcc > 5.f)
	{
		GAMEINSTANCE->Play_Sound(L"BGM_Clear", (CHANNEL_GROUP)CHANNEL_UI);
		On_Clear();
		m_fTimeAcc = 0.f;
		m_bClearUITime = false;
	}
}

void CUI_Intro::My_LateTick()
{

}
