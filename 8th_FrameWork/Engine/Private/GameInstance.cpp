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
#include "Prototype_Manager.h"
#include "CComponent_Manager.h"

#define MGR(type) type::Get_Instance()

CGameInstance::CGameInstance()
{
}

CGameInstance::~CGameInstance()
{
	Release();
}

ComPtr<ID3D11Device>	CGameInstance::Get_Device()
{
	return MGR(CGraphic_Device)->Get_Device();
}

ComPtr<ID3D11DeviceContext> CGameInstance::Get_DeviceContext()
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
	//MGR(CObject_Manager)->Tick();
	MGR(CComponent_Manager)->Tick();

	MGR(CLevel_Manager)->Late_Tick();
	//MGR(CObject_Manager)->Late_Tick();
	MGR(CComponent_Manager)->Late_Tick();

	/* ViewProj */
	MGR(CCamera_Manager)->Make_ViewProj();

	/* Frustum */
	//MGR(CZFrustum)->Make_Plane(MGR(CCamera_Manager)->Get_ViewProj());

	/* Other Events */
	//MGR(CPicking_Manager)->Execute_Picking();
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

HRESULT CGameInstance::Clear_BackBuffer_View(_float4 vClearColor)
{
	return MGR(CGraphic_Device)->Clear_BackBuffer_View(vClearColor);
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{
	return MGR(CGraphic_Device)->Clear_DepthStencil_View();
}

HRESULT CGameInstance::Present()
{
	return MGR(CGraphic_Device)->Present();
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

_double CGameInstance::Get_DT()
{
	return MGR(CTime_Manager)->Get_DT();
}

KEY_STATE CGameInstance::Get_KeyState(KEY _key)
{
	return MGR(CKey_Manager)->Get_KeyState(_key);
}

CGameObject* CGameInstance::Get_StaticObj(const _uint& iKeyValue)
{
	return MGR(CObject_Manager)->Get_StaticObj(iKeyValue);
}

list<CGameObject*>& CGameInstance::Get_ObjGroup(const _uint& iGroupIdx)
{
	return MGR(CObject_Manager)->Get_ObjGroup(iGroupIdx);
}

void CGameInstance::Delete_Objects(const _uint& iGroupIdx)
{
	return MGR(CObject_Manager)->Delete_Objects(iGroupIdx);
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

void CGameInstance::Create_StaticObject(CGameObject * pGameObject, const _uint& iObjectID)
{
	MGR(CEvent_Manager)->Create_StaticObject(pGameObject, iObjectID);
}

void CGameInstance::Change_Level(CLevel* pLevel)
{
	MGR(CEvent_Manager)->Change_Level(pLevel);
}

CGameObject* CGameInstance::Clone_GameObject(const _uint& _iID)
{
	return MGR(CPrototype_Manager)->Clone_GameObject(_iID);
}

CComponent* CGameInstance::Clone_Component(const _uint& _iID)
{
	return MGR(CPrototype_Manager)->Clone_Component(_iID);
}

HRESULT CGameInstance::Add_GameObject_Prototype(const _uint& _iID, CGameObject* pGameObject)
{
	return MGR(CPrototype_Manager)->Add_GameObject_Prototype(_iID, pGameObject);
}

HRESULT CGameInstance::Add_Component_Prototype(const _uint& _iID, CComponent* pComponent)
{
	return MGR(CPrototype_Manager)->Add_Component_Prototype(_iID, pComponent);
}
