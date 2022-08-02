#include "stdafx.h"
#include "CCamera_Free.h"

CCamera_Free::CCamera_Free()
{
}

CCamera_Free::~CCamera_Free()
{
}

HRESULT CCamera_Free::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	m_iID = 100;

	return S_OK;
}

HRESULT CCamera_Free::Initialize()
{

	return S_OK;
}
