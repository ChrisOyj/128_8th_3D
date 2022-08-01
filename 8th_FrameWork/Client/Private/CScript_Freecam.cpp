#include "stdafx.h"
#include "CScript_Freecam.h"

#include "GameInstance.h"

CScript_Freecam::~CScript_Freecam()
{
}

HRESULT CScript_Freecam::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CScript_Freecam::Initialize()
{
	return S_OK;
}

void CScript_Freecam::Tick()
{
	if (KEY(W, HOLD))
	{

	}
}

void CScript_Freecam::Late_Tick()
{
}

void CScript_Freecam::Release()
{
}
