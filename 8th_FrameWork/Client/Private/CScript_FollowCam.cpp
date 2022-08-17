#include "stdafx.h"
#include "CScript_FollowCam.h"

#include "GameInstance.h"

#include "GameObject.h"
#include "Transform.h"

#include "CUtility_Transform.h"
#include "Loading_Manager.h"

CScript_FollowCam::CScript_FollowCam(_uint iGroupIdx)
	: CScriptable(iGroupIdx)
{
}

CScript_FollowCam::~CScript_FollowCam()
{
}

CScript_FollowCam* CScript_FollowCam::Create(_uint iGroupIdx)
{
	CScript_FollowCam* pInstance = new CScript_FollowCam(iGroupIdx);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Create CScript_FollowCam");
		delete pInstance;
		return nullptr;
	}

	return pInstance;
}

HRESULT CScript_FollowCam::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CScript_FollowCam::Initialize()
{
	return S_OK;
}

void CScript_FollowCam::Tick()
{
}

void CScript_FollowCam::Late_Tick()
{
}

void CScript_FollowCam::Release()
{
}

void CScript_FollowCam::OnDisable()
{
	__super::OnDisable();
}
