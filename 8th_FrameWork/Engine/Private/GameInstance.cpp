#include "..\Public\GameInstance.h"
#include "Input_Device.h"
#include "Picking_Manager.h"

#define FINDMGR(type) type::Get_Instance()

CGameInstance::CGameInstance()
{
}

CGameInstance::~CGameInstance()
{
	Release();
}

LPDIRECT3DDEVICE9	CGameInstance::Get_Device()
{
	return FINDMGR(CGraphic_Device)->Get_Device();
}

LPD3DXSPRITE CGameInstance::Get_Sprite(void)
{
	return FINDMGR(CGraphic_Device)->Get_Sprite();
}

LPD3DXFONT CGameInstance::Get_Font(void)
{
	return FINDMGR(CGraphic_Device)->Get_Font();
}

HRESULT CGameInstance::Initialize_Engine(const GRAPHICDESC& GraphicDesc)
{
	Initialize();

	/* 그래픽디바이스. */
	if (FAILED(FINDMGR(CGraphic_Device)->InitDevice(GraphicDesc)))
		return E_FAIL;  

	/* 인풋 디바이스. */
	if (FAILED(FINDMGR(CKey_Manager)->Initialize(GraphicDesc.hWnd)))
		return E_FAIL;

	if (FAILED(FINDMGR(CInput_Device)->Initialize(GraphicDesc.hInst, GraphicDesc.hWnd)))
		return E_FAIL;

	/* FMOD */
	if (FAILED(FINDMGR(CSound_Device)->Initialize()))
		return E_FAIL;

	/* Time Manager */
	if (FAILED(FINDMGR(CTime_Manager)->Initialize(GraphicDesc.hWnd)))
		return E_FAIL;

	/* Camera Manager*/
	if (FAILED(FINDMGR(CCamera_Manager)->Initialize(GraphicDesc)))
		return E_FAIL;

	/* Collision Manager */
	if (FAILED(FINDMGR(CCollision_Manager)->Initialize()))
		return E_FAIL;

	if (FAILED(FINDMGR(CShader_Manager)->Initialize()))
		return E_FAIL;

	/* Picking Manager */
	if (FAILED(FINDMGR(CPicking_Manager)->Initialize(GraphicDesc.hWnd)))
		return E_FAIL;

	return S_OK;	
}

HRESULT CGameInstance::Tick_Engine( )
{
	/* Event */
	FINDMGR(CEvent_Manager)->Tick();

	/* Time, Key */
	FINDMGR(CTime_Manager)->Tick();
	FINDMGR(CKey_Manager)->Tick();
	FINDMGR(CInput_Device)->SetUp_DeviceStates();

	
	/* Object */
	FINDMGR(CLevel_Manager)->Tick();
	FINDMGR(CCamera_Manager)->Tick();
	FINDMGR(CObject_Manager)->Tick();

	FINDMGR(CObject_Manager)->Late_Tick();
	FINDMGR(CCamera_Manager)->Late_Tick();
	FINDMGR(CLevel_Manager)->Late_Tick();

	/* Frustum */
	FINDMGR(CZFrustum)->Make_Plane(FINDMGR(CCamera_Manager)->Get_ViewProj());

	/* Other Events */
	FINDMGR(CPicking_Manager)->Tick();
	FINDMGR(CCollision_Manager)->Tick();



	return S_OK;
}

HRESULT CGameInstance::Render_Engine()
{
	if (FAILED(FINDMGR(CRender_Manager)->Render()))
		return E_FAIL;

	FINDMGR(CTime_Manager)->Render();

	return S_OK;
}

HRESULT CGameInstance::Initialize()
{


	return S_OK;
}

void CGameInstance::Release()
{
}

void CGameInstance::Render_Begin(void)
{
	return FINDMGR(CGraphic_Device)->Render_Begin();
}

void CGameInstance::Render_End()
{
	return FINDMGR(CGraphic_Device)->Render_End();
}

_long CGameInstance::Get_DIMouseMoveState(MOUSEMOVE eMouseMove)
{
	return FINDMGR(CInput_Device)->Get_DIMouseMoveState(eMouseMove);
}

void CGameInstance::Play_Sound(const _tchar * strSoundKey, _float fVolume)
{
	wstring strKey = strSoundKey;
	strKey += L".wav";

	FINDMGR(CSound_Device)->Play_Sound(strKey.c_str(), fVolume);
}

void CGameInstance::Play_Sound_Rand(const _tchar * strSoundKey, const _uint & iRandNum, _float fVolume)
{
	wstring strKey = strSoundKey;

	_int iRand = random(0, iRandNum);
	_tchar szTemp[MIN_STR] = {};

	_itow_s(iRand, szTemp, 10);

	strKey += szTemp;
	strKey += L".wav";

	FINDMGR(CSound_Device)->Play_Sound(strKey.c_str(), fVolume);
}

void CGameInstance::Play_Sound_Player(const _tchar * strSoundKey, _float fVolume)
{
	wstring strKey = strSoundKey;
	strKey += L".wav";

	FINDMGR(CSound_Device)->Play_Sound_Player(strKey.c_str(), fVolume);
}

void CGameInstance::Play_Sound_Player_Rand(const _tchar * strSoundKey, const _uint & iRandNum, _float fVolume)
{
	wstring strKey = strSoundKey;

	_int iRand = random(0, iRandNum);
	_tchar szTemp[MIN_STR] = {};

	_itow_s(iRand, szTemp, 10);

	strKey += szTemp;
	strKey += L".wav";

	FINDMGR(CSound_Device)->Play_Sound_Player(strKey.c_str(), fVolume);
}

void CGameInstance::PlayBGM(const _tchar * strSoundKey, _float fVolume)
{
	wstring strKey = strSoundKey;
	strKey += L".wav";
	FINDMGR(CSound_Device)->PlayBGM(strKey.c_str(), fVolume);
}

void CGameInstance::StopSound(CHANNELID eType)
{
	FINDMGR(CSound_Device)->StopSound(eType);
}

void CGameInstance::StopAll()
{
	FINDMGR(CSound_Device)->StopAll();
}

void CGameInstance::SetVolume(_float fVolume)
{
	FINDMGR(CSound_Device)->SetVolume(fVolume);
}

void CGameInstance::SetChannelVolume(CHANNELID eID, _float fVolume)
{
	FINDMGR(CSound_Device)->SetChannelVolume(eID, fVolume);
}

_double CGameInstance::Get_DT(bool bReal)
{
	return FINDMGR(CTime_Manager)->Get_DT(bReal);
}

KEY_STATE CGameInstance::Get_KeyState(KEY _key)
{
	return FINDMGR(CKey_Manager)->Get_KeyState(_key);
}

void CGameInstance::Add_Camera(wstring strKey, CCamera * pCamera)
{
	FINDMGR(CCamera_Manager)->Add_Camera(strKey, pCamera);
}

CCamera * CGameInstance::Change_Camera(wstring strKey)
{
	return FINDMGR(CCamera_Manager)->Change_Camera(strKey);
}

void CGameInstance::Delete_GameObject(CGameObject * pGameObject)
{
	FINDMGR(CEvent_Manager)->Delete_GameObject(pGameObject);
}

void CGameInstance::Create_GameObject(CGameObject * pGameObject)
{
	FINDMGR(CEvent_Manager)->Create_GameObject(pGameObject);
}

void CGameInstance::Enable_GameObject(CGameObject * pGameObject)
{
	FINDMGR(CEvent_Manager)->Enable_GameObject(pGameObject);
}

void CGameInstance::Disable_GameObject(CGameObject * pGameObject)
{
	FINDMGR(CEvent_Manager)->Disable_GameObject(pGameObject);
}

void CGameInstance::Create_StaticObject(CGameObject * pGameObject, _hashcode hashcode)
{
	FINDMGR(CEvent_Manager)->Create_StaticObject(pGameObject, hashcode);
}

void CGameInstance::Change_Level(_uint iLevelID, CLevel * pLevel)
{
	FINDMGR(CEvent_Manager)->Change_Level(iLevelID, pLevel);
}

HRESULT CGameInstance::Set_Parameter(const CHAR * parameterName, const _float * pFloats, _uint iNum)
{
	if (FAILED(FINDMGR(CShader_Manager)->Set_Parameter(parameterName, pFloats, iNum)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGameInstance::Set_Texture(LPDIRECT3DBASETEXTURE9 pTexture)
{
	if (FAILED(FINDMGR(CShader_Manager)->Set_Texture(pTexture)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGameInstance::Set_float4x4(const CHAR * parameterName, const _float4x4 & _float4x4)
{
	FINDMGR(CShader_Manager)->Set_float4x4(parameterName, _float4x4);

	return S_OK;
}

void CGameInstance::Add_PickingObject(CGameObject * pGameObject, _float fDistanceToPlayer)
{
	FINDMGR(CPicking_Manager)->Add_Object(pGameObject, fDistanceToPlayer);
}

void CGameInstance::Add_PickingUI(CUI * pUI)
{
	FINDMGR(CPicking_Manager)->Add_UI(pUI);

}

void CGameInstance::Execute_Picking()
{
	FINDMGR(CPicking_Manager)->Execute_Picking();
}

void CGameInstance::SetRayCenterAnyTime(_bool bBool)
{
	FINDMGR(CPicking_Manager)->SetRayCenterAnyTime(bBool);
}

_bool CGameInstance::GetRayCenterAnyTime()
{
	return FINDMGR(CPicking_Manager)->GetRayCenterAnyTime();
}

void CGameInstance::Execute_RandomPicking()
{
	FINDMGR(CPicking_Manager)->Execute_RandomPicking();
}

_bool CGameInstance::IsOnMouseRay(CGameObject * pGameObject)
{
	return FINDMGR(CPicking_Manager)->IsOnMouseRay(pGameObject);
}

_bool CGameInstance::Is3DPicked(CGameObject * pGameObject, _float3 * pOut, _float3 * pOutNormal)
{
	return FINDMGR(CPicking_Manager)->Is3DPicked(pGameObject, pOut, pOutNormal);
}

_bool CGameInstance::IsInPlane(const _float3 & vPos)
{
	return FINDMGR(CZFrustum)->IsInPlane(vPos);
}

_bool CGameInstance::IsInSphere(const _float3 & vPos, const _float& fRadius)
{
	return FINDMGR(CZFrustum)->IsInSphere(vPos, fRadius);
}
