#include "stdafx.h"

#include "GameInstance.h"

#include "CLevel_Loading.h"
#include "CLevel_Default.h"

#include "Loading_Manager.h"

#include "CPrototype_Factory.h"
#include "CGameObject_Factory.h"
#include "CSimple_Image.h"

#include "CFader.h"
#include "CShader.h"
#include "Renderer.h"

#define	LOADING_IMAGE_PROTOTYPE_ID		111112
#define	LOADINGBAR_IMAGE_PROTOTYPE_ID		111113

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
	if (FAILED(CGameInstance::Get_Instance()->Add_Font(TEXT("Font_Arial"), TEXT("../Bin/Resources/Fonts/125.spriteFont"))))
		return E_FAIL;

	if (FAILED(CPrototype_Factory::SetUp_DefaultComponents()))
		return E_FAIL;

	/* Loading Image */
	FADEDESC	tFadeDesc;
	tFadeDesc.bFadeFlag = FADE_KEY;
	tFadeDesc.eKeyType = KEY::ENTER;
	tFadeDesc.fAlpha = 0.f;
	tFadeDesc.fFadeInTime = 2.f;
	tFadeDesc.fFadeOutTime = 2.f;
	tFadeDesc.fFadeOutStartTime = 3.f;

	CSimple_Image* pLoadingImage = CSimple_Image::Create(_float4(g_iWinCX * 0.5f, g_iWinCY * 0.5f, 0.f, 1.f), _float2((_float)g_iWinCX, (_float)g_iWinCY),
		L"../Bin/resources/textures/UI/Loading_Screen/T_UI_LoadingScreen_%.3d_BC.png", 10,
		CSimple_Image::SIMPLE_LOADING, tFadeDesc, CSimple_Image::FADER_LOADING);

	if (!pLoadingImage)
		return E_FAIL;

	CGameInstance::Get_Instance()->Add_GameObject_Prototype(LOADING_IMAGE_PROTOTYPE_ID, pLoadingImage);


	/* Loading Bar */
	CSimple_Image* pLoadingBar = CSimple_Image::Create(_float4(g_iWinCX * 0.5f, g_iWinCY * 0.9f, 0.f, 1.f), _float2((_float)g_iWinCX * 0.75f, 2.f),
		L"../Bin/resources/textures/UI/Loading_Screen/T_UI_LoadIndicator_Base_BC.png", 1,
		CSimple_Image::SIMPLE_LOADINGBAR, tFadeDesc, CSimple_Image::FADER_LOADING);

	if (!pLoadingImage)
		return E_FAIL;

	pLoadingBar->Get_Component<CRenderer>()[0]->Set_Pass(VTXTEX_PASS_LOADINGBAR);
	CGameInstance::Get_Instance()->Add_GameObject_Prototype(LOADINGBAR_IMAGE_PROTOTYPE_ID, pLoadingBar);

	return S_OK;
}

HRESULT CLevel_Loading::Enter()
{
	CGameObject* pLoadingImage = CGameObject_Factory::Create_FromPrototype(LOADING_IMAGE_PROTOTYPE_ID);
	_uint iRand = random(0, 9);
	static_cast<CSimple_Image*>(pLoadingImage)->Set_TextureIdx(iRand);
	CREATE_GAMEOBJECT(pLoadingImage, GROUP_LOADING);


	CGameObject* pLoadingBar = CGameObject_Factory::Create_FromPrototype(LOADINGBAR_IMAGE_PROTOTYPE_ID);
	CREATE_GAMEOBJECT(pLoadingBar, GROUP_LOADING);




	CGameInstance::Get_Instance()->Change_Camera(L"Default");

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

	return S_OK;
}



