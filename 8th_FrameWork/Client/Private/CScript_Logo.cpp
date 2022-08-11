#include "stdafx.h"
#include "CScript_Logo.h"

#include "GameInstance.h"

#include "GameObject.h"
#include "Transform.h"

#include "CUtility_Transform.h"

#include "Loading_Manager.h"

CScript_Logo::CScript_Logo(_uint iGroupIdx)
	: CScriptable(iGroupIdx)
{
}

CScript_Logo::~CScript_Logo()
{
}

CScript_Logo* CScript_Logo::Create(_uint iGroupIdx, LEVEL_TYPE_CLIENT eNextLevelType)
{
	CScript_Logo* pInstance = new CScript_Logo(iGroupIdx);

	pInstance->m_eNextLevelType = eNextLevelType;

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
	CLoading_Manager::Get_Instance()->Reserve_Load_Level(m_eNextLevelType);
}
