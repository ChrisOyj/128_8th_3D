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
	m_iID = 111111;

	__super::Initialize_Prototype();

	CScript_Freecam* pScriptable = CScript_Freecam::Create(CP_AFTER_TRANSFORM);
	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCRIPT_FREECAM_PROTOTYPE_ID, pScriptable)))
		return E_FAIL;
	Add_Component(CComponent_Factory::Create_FromPrototype(SCRIPT_FREECAM_PROTOTYPE_ID, this));


	return S_OK;
}

HRESULT CCamera_Free::Initialize()
{

	return S_OK;
}
