#include "stdafx.h"
#include "CScript_Logo.h"

#include "GameInstance.h"

#include "GameObject.h"
#include "Transform.h"

#include "CUtility_Transform.h"
#include "Loading_Manager.h"

#include "CFader.h"

CScript_Logo::CScript_Logo(_uint iGroupIdx)
	: CScriptable(iGroupIdx)
{
}

CScript_Logo::~CScript_Logo()
{
}

CScript_Logo* CScript_Logo::Create(_uint iGroupIdx, CGameObject* pBG)
{
	CScript_Logo* pInstance = new CScript_Logo(iGroupIdx);

	pInstance->m_pBG = pBG;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Create CScript_Logo");
		delete pInstance;
		return nullptr;
	}

	return pInstance;
}

HRESULT CScript_Logo::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CScript_Logo::Initialize()
{
	return S_OK;
}

void CScript_Logo::Tick()
{
	if (KEY(ENTER, TAP))
	{
		GET_COMPONENT_FROM(m_pOwner, CFader)->Get_FastTime() *= 2.f;
	}
}

void CScript_Logo::Late_Tick()
{
}

void CScript_Logo::Release()
{
}

void CScript_Logo::OnDisable()
{
	__super::OnDisable();

	if (m_pBG)
		DELETE_GAMEOBJECT(m_pBG);

	DELETE_GAMEOBJECT(m_pOwner);

	CHANGE_LEVEL(CLoading_Manager::Get_Instance()->Get_Level(LEVEL_MAINMENU));
}
