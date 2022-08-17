#include "stdafx.h"
#include "CLevel_Loading.h"

#include "GameInstance.h"

#include "Loading_Manager.h"

#include "CPrototype_Factory.h"
#include "CGameObject_Factory.h"
#include "CComponent_Factory.h"

#include "CDarkScreen.h"
#include "CLoading_BG.h"
#include "CLoading_Turn.h"
#include "CLoading_Bar.h"

#include "CFader.h"
#include "CShader.h"
#include "Renderer.h"
#include "Physics.h"

#include "Functor.h"

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
	CGameInstance::Get_Instance()->Add_GameObject_Prototype(CDarkScreen::Create());

	CLoading_BG* pLoadingBG = CLoading_BG::Create();
	CGameInstance::Get_Instance()->Add_GameObject_Prototype(pLoadingBG);

	_float4 vPos = _float4(g_iWinCX*0.5f, g_iWinCY*0.87f, 0.f, 1.f);
	CGameInstance::Get_Instance()->Add_GameObject_Prototype(CLoading_Bar::Create(vPos));

	vPos.x += g_iWinCX * 0.4f;
	CGameInstance::Get_Instance()->Add_GameObject_Prototype(CLoading_Turn::Create(vPos));

	return S_OK;
}

HRESULT CLevel_Loading::SetUp_Prototypes()
{
	return S_OK;
}

HRESULT CLevel_Loading::Enter()
{
	Ready_GameObject(CGameObject_Factory::Clone_GameObject<CDarkScreen>(), GROUP_LOADING);
	Ready_GameObject(CGameObject_Factory::Clone_GameObject<CLoading_BG>(), GROUP_LOADING);
	Ready_GameObject(CGameObject_Factory::Clone_GameObject<CLoading_Turn>(), GROUP_LOADING);
	Ready_GameObject(CGameObject_Factory::Clone_GameObject<CLoading_Bar>(), GROUP_LOADING);

	CGameObject* pLoadingBar = m_vecGameObjects.back().first;

	/* bind g_fProgress */
	if (!(m_pLoadingBarShader = GET_COMPONENT_FROM(pLoadingBar, CShader)))
		return E_FAIL;

	m_pLoadingBarShader->CallBack_SetRawValues += bind(&CLoading_Manager::Set_ShaderResource, CLoading_Manager::Get_Instance()
		, placeholders::_1, "g_fProgress");

	CGameInstance::Get_Instance()->Change_Camera(L"Default");

	__super::Enter();


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
	_tchar szTemp[MIN_STR];

	if (true == CLoading_Manager::Get_Instance()->IsFinished())
	{
		swprintf_s(szTemp, L"로딩끝");
	}
	else
		swprintf_s(szTemp, L"로딩중");


	if (FAILED(CGameInstance::Get_Instance()->Render_Font(TEXT("Font_Arial"), szTemp, _float2(10.f, 10.f), _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Loading::Exit()
{
	CGameInstance::Get_Instance()->Delete_Objects(GROUP_LOADING);
	CGameInstance::Get_Instance()->Clear_All_Components();

	__super::Exit();

	return S_OK;
}



