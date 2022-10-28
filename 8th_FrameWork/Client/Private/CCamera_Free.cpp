#include "stdafx.h"
#include "CCamera_Free.h"

#include "GameInstance.h"

#include "CScript_Freecam.h"
#include "CComponent_Factory.h"

CCamera_Free::CCamera_Free()
{
}

CCamera_Free::~CCamera_Free()
{
}

HRESULT CCamera_Free::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	CScript_Freecam* pScriptable = CScript_Freecam::Create(CP_AFTER_TRANSFORM);
	pScriptable->Set_Owner(this);
	Add_Component(pScriptable);
	m_tProj.fFOV = XMConvertToRadians(55.f);


	return S_OK;
}

HRESULT CCamera_Free::Initialize()
{

	return S_OK;
}
