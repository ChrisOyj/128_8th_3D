#include "stdafx.h"

#include "GameInstance.h"

#include "CLevel_Loading.h"
#include "CLevel_Default.h"

#include "Loading_Manager.h"

CLevel_Loading::CLevel_Loading()
{
}

CLevel_Loading* CLevel_Loading::Create()
{
	CLevel_Loading* pInstance = new CLevel_Loading();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CLevel_Loading");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	return pInstance;
}



HRESULT CLevel_Loading::Initialize()
{
	return S_OK;
}

HRESULT CLevel_Loading::Enter()
{
	return S_OK;
}

void CLevel_Loading::Tick()
{
	if (true == CLoading_Manager::Get_Instance()->IsFinished())
	{
		if (KEY(ENTER, TAP))
		{
			CLoading_Manager::Get_Instance()->Open_NextLevel();
		}
	}
}

void CLevel_Loading::Late_Tick()
{
}

HRESULT CLevel_Loading::Render()
{

	return S_OK;
}

HRESULT CLevel_Loading::Exit()
{

	return S_OK;
}



