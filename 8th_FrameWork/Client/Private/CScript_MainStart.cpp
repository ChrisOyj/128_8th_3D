#include "stdafx.h"
#include "CScript_MainStart.h"

#include "GameInstance.h"

#include "GameObject.h"
#include "Transform.h"

#include "CUtility_Transform.h"
#include "Loading_Manager.h"

#include "CFader.h"

#include "CDefault_UI.h"
#include "CGameObject_Factory.h"

CScript_MainStart::CScript_MainStart(_uint iGroupIdx)
	: CScriptable(iGroupIdx)
{
}

CScript_MainStart::~CScript_MainStart()
{
}

CScript_MainStart* CScript_MainStart::Create(_uint iGroupIdx, vector<CGameObject*>& vecUIs)
{
	CScript_MainStart* pInstance = new CScript_MainStart(iGroupIdx);

	pInstance->m_vecUIs = vecUIs;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Create CScript_MainStart");
		SAFE_DELETE(pInstance)
	}

	return pInstance;
}

HRESULT CScript_MainStart::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CScript_MainStart::Initialize()
{
	return S_OK;
}

void CScript_MainStart::Tick()
{
	CFader* pFader = GET_COMPONENT_FROM(m_pOwner, CFader);
	

	if (!m_bOnce && pFader->Get_FadeDesc().fAlpha == 1.f)
	{
		m_bOnce = true;
		m_pEffects[0] = CGameObject_Factory::Create_UI(L"MAINMENU2_StartButtonEffect");
		m_pEffects[1] = CGameObject_Factory::Create_UI(L"MAINMENU2_TITLEEffect");
		CREATE_GAMEOBJECT(m_pEffects[0], GROUP_UI);
		CREATE_GAMEOBJECT(m_pEffects[1], GROUP_UI);
	}

	if (KEY(ENTER, TAP))
	{
		if (!m_bOnce)
			return;

		DELETE_GAMEOBJECT(m_pEffects[0]);
		DELETE_GAMEOBJECT(m_pEffects[1]);

		for (auto& elem : m_vecUIs)
		{
			CFader* pFader = GET_COMPONENT_FROM(elem, CFader);
			pFader->Force_KeyInput();
			pFader->Get_FadeDesc().eFadeOutType = FADEDESC::FADEOUT_DISABLE;
		}

		pFader->Force_KeyInput();
		pFader->Get_FadeDesc().eFadeOutType = FADEDESC::FADEOUT_DISABLE;

		GAMEINSTANCE->Play_Sound(L"Select_Big", (CHANNEL_GROUP)CHANNEL_UI);


	}
}

void CScript_MainStart::Late_Tick()
{
}

void CScript_MainStart::Release()
{
}

void CScript_MainStart::OnDisable()
{
	__super::OnDisable();

	CHANGE_LEVEL(CLoading_Manager::Get_Instance()->Get_Level(LEVEL_CUSTOMIZING));

}
