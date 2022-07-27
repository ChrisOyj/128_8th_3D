#include "..\Public\GameInstance.h"

#include "Graphic_Device.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "Render_Manager.h"
#include "Event_Manager.h"
#include "Time_Manager.h"
#include "Key_Manager.h"
#include "Camera_Manager.h"
#include "Collision_Manager.h"
#include "ZFrustum.h"
#include "Sound_Device.h"
#include "Input_Device.h"
#include "Picking_Manager.h"

#define MGR(type) type::Get_Instance()

CGameInstance::CGameInstance()
{
}

CGameInstance::~CGameInstance()
{
	Release();
}

ID3D11Device*	CGameInstance::Get_Device()
{
	return MGR(CGraphic_Device)->Get_Device();
}

ID3D11DeviceContext* CGameInstance::Get_DeviceContext()
{
	return MGR(CGraphic_Device)->Get_DeviceContext();
}

HRESULT CGameInstance::Initialize_Engine(const GRAPHICDESC& GraphicDesc, const SOUNDDESC& SoundDesc)
{
	Initialize();

	/* 그래픽디바이스. */
	if (FAILED(MGR(CGraphic_Device)->Ready_Graphic_Device(GraphicDesc)))
		return E_FAIL;  

	/* 인풋 디바이스. */
	if (FAILED(MGR(CKey_Manager)->Initialize(GraphicDesc.hWnd)))
		return E_FAIL;

	if (FAILED(MGR(CInput_Device)->Initialize(GraphicDesc.hInst, GraphicDesc.hWnd)))
		return E_FAIL;

	/* FMOD */
	if (FAILED(MGR(CSound_Device)->Initialize(SoundDesc)))
		return E_FAIL;

	/* Time Manager */
	if (FAILED(MGR(CTime_Manager)->Initialize(GraphicDesc.hWnd)))
		return E_FAIL;

	/* Camera Manager*/
	if (FAILED(MGR(CCamera_Manager)->Initialize(GraphicDesc)))
		return E_FAIL;

	/* Collision Manager */
	if (FAILED(MGR(CCollision_Manager)->Initialize()))
		return E_FAIL;

	/* Picking Manager */
	if (FAILED(MGR(CPicking_Manager)->Initialize(GraphicDesc)))
		return E_FAIL;

	return S_OK;	
}

HRESULT CGameInstance::Tick_Engine( )
{
	/* Event */
	MGR(CEvent_Manager)->Tick();

	/* Time, Key */
	MGR(CTime_Manager)->Tick();
	MGR(CKey_Manager)->Tick();
	MGR(CInput_Device)->SetUp_DeviceStates();

	/* Object */
	MGR(CLevel_Manager)->Tick();
	MGR(CObject_Manager)->Tick();

	MGR(CObject_Manager)->Late_Tick();
	MGR(CLevel_Manager)->Late_Tick();

	/* ViewProj */
	MGR(CCamera_Manager)->Make_ViewProj();

	/* Frustum */
	//MGR(CZFrustum)->Make_Plane(MGR(CCamera_Manager)->Get_ViewProj());

	/* Other Events */
	MGR(CPicking_Manager)->Execute_Picking();
	MGR(CCollision_Manager)->Tick();



	return S_OK;
}

HRESULT CGameInstance::Render_Engine()
{
	if (FAILED(MGR(CRender_Manager)->Render()))
		return E_FAIL;

	MGR(CTime_Manager)->Render();

	return S_OK;
}

HRESULT CGameInstance::Initialize()
{


	return S_OK;
}

void CGameInstance::Release()
{
}

_long CGameInstance::Get_DIMouseMoveState(MOUSEMOVE eMouseMove)
{
	return MGR(CInput_Device)->Get_DIMouseMoveState(eMouseMove);
}

void CGameInstance::Play_Sound(const _tchar* strSoundKey, CHANNEL_GROUP iGroupIndex, _float fVolumeRatio)
{
	wstring strKey = strSoundKey;
	strKey += L".wav";

	MGR(CSound_Device)->Play_Sound(strKey.c_str(), iGroupIndex, fVolumeRatio);
}

void CGameInstance::Play_Sound_Rand(const _tchar* strSoundKey, const _uint& iRandCnt, CHANNEL_GROUP iGroupIndex, _float fVolumeRatio)
{
	wstring strKey = strSoundKey;

	_int iRand = random(0, iRandCnt);
	_tchar szTemp[MIN_STR] = {};

	_itow_s(iRand, szTemp, 10);

	strKey += szTemp;
	strKey += L".wav";

	MGR(CSound_Device)->Play_Sound(strKey.c_str(), iGroupIndex, fVolumeRatio);
}

void CGameInstance::Play_BGM(const _tchar* strSoundKey)
{
	wstring strKey = strSoundKey;
	strKey += L".wav";
	MGR(CSound_Device)->Play_BGM(strKey.c_str());
}

void CGameInstance::Stop_Sound(CHANNEL_GROUP eType)
{
	MGR(CSound_Device)->Stop_Sound(eType);

}

void CGameInstance::Stop_All()
{
	MGR(CSound_Device)->Stop_All();
}

void CGameInstance::Set_Volume(_float fVolume)
{
	MGR(CSound_Device)->Set_Volume(fVolume);
}

void CGameInstance::Set_ChannelVolume(CHANNEL_GROUP  eID, _float fVolume)
{
	MGR(CSound_Device)->Set_ChannelVolume(eID, fVolume);
}

void CGameInstance::Set_ChannelVolume(CHANNEL_GROUP eID, const _uint& iChannelIndex, _float fVolume)
{
	MGR(CSound_Device)->Set_ChannelVolume(eID, iChannelIndex, fVolume);
}

HRESULT CGameInstance::Reserve_Level(const _uint& iLevelSize)
{
	return MGR(CLevel_Manager)->Reserve_Level(iLevelSize);
}

HRESULT CGameInstance::Add_Level(CLevel* pLevel)
{
	return MGR(CLevel_Manager)->Add_Level(pLevel);
}

_bool CGameInstance::Is_AllLevelsReady()
{
	return MGR(CLevel_Manager)->Is_AllLevelsReady();
}


_double CGameInstance::Get_DT(_bool bReal)
{
	return MGR(CTime_Manager)->Get_DT(bReal);
}

KEY_STATE CGameInstance::Get_KeyState(KEY _key)
{
	return MGR(CKey_Manager)->Get_KeyState(_key);
}

void CGameInstance::Add_Camera(wstring strKey, CCamera * pCamera)
{
	MGR(CCamera_Manager)->Add_Camera(strKey, pCamera);
}

CCamera * CGameInstance::Change_Camera(wstring strKey)
{
	return MGR(CCamera_Manager)->Change_Camera(strKey);
}

CCamera* CGameInstance::Get_CurCam()
{
	return MGR(CCamera_Manager)->Get_CurCam();
}

_float4 CGameInstance::Get_ViewPos()
{
	return MGR(CCamera_Manager)->Get_ViewPos();
}

void CGameInstance::Delete_GameObject(CGameObject * pGameObject)
{
	MGR(CEvent_Manager)->Delete_GameObject(pGameObject);
}

void CGameInstance::Create_GameObject(CGameObject* pGameObject, const _uint& iGroupIdx)
{
	MGR(CEvent_Manager)->Create_GameObject(pGameObject, iGroupIdx);

}

void CGameInstance::Delete_Component(CComponent* pComponent, CGameObject* pGameObject)
{
	MGR(CEvent_Manager)->Delete_Component(pComponent, pGameObject);

}

void CGameInstance::Create_Component(CComponent* pComponent, CGameObject* pGameObject)
{
	MGR(CEvent_Manager)->Create_Component(pComponent, pGameObject);
}

void CGameInstance::Enable_GameObject(CGameObject * pGameObject)
{
	MGR(CEvent_Manager)->Enable_GameObject(pGameObject);
}

void CGameInstance::Disable_GameObject(CGameObject * pGameObject)
{
	MGR(CEvent_Manager)->Disable_GameObject(pGameObject);
}

void CGameInstance::Disable_Component(CComponent* pComponent)
{
	MGR(CEvent_Manager)->Disable_Component(pComponent);

}

void CGameInstance::Enable_Component(CComponent* pComponent)
{
	MGR(CEvent_Manager)->Enable_Component(pComponent);

}

void CGameInstance::Create_StaticObject(CGameObject * pGameObject, _hashcode hashcode)
{
	MGR(CEvent_Manager)->Create_StaticObject(pGameObject, hashcode);
}

void CGameInstance::Change_Level(_uint iLevelID)
{
	MGR(CEvent_Manager)->Change_Level(iLevelID);
}