#include "..\Public\GameInstance.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{
}

CGameInstance::~CGameInstance()
{
	Release();
}

ComPtr<ID3D11Device>	CGameInstance::Get_Device()
{
	return m_pGraphicDevice->Get_Device();
}

ComPtr<ID3D11DeviceContext> CGameInstance::Get_DeviceContext()
{
	return m_pGraphicDevice->Get_DeviceContext();
}

HRESULT CGameInstance::Initialize_Engine(const GRAPHICDESC& GraphicDesc, const SOUNDDESC& SoundDesc)
{
	Initialize();

	/* �׷��ȵ���̽�. */
	if (FAILED(m_pGraphicDevice->Ready_Graphic_Device(GraphicDesc)))
		return E_FAIL;  

	/* ��ǲ ����̽�. */
	if (FAILED(m_pKeyManager->Initialize(GraphicDesc.hWnd)))
		return E_FAIL;

	if (FAILED(m_pInputDevice->Initialize(GraphicDesc.hInst, GraphicDesc.hWnd)))
		return E_FAIL;

	/* FMOD */
	if (FAILED(m_pSoundDevice->Initialize(SoundDesc)))
		return E_FAIL;

	/* Time Manager */
	if (FAILED(m_pTimeManager->Initialize(GraphicDesc.hWnd)))
		return E_FAIL;

	/* Camera Manager*/
	if (FAILED(m_pCameraManager->Initialize(GraphicDesc)))
		return E_FAIL;

	/* Collision Manager */
	if (FAILED(m_pCollisionManager->Initialize()))
		return E_FAIL;

	/* Picking Manager */
	if (FAILED(m_pPickingManager->Initialize(GraphicDesc)))
		return E_FAIL;

	return S_OK;	
}

HRESULT CGameInstance::Tick_Engine( )
{
	/* Event */
	m_pEventManager->Tick();

	/* Time, Key */
	m_pTimeManager->Tick();
	m_pKeyManager->Tick();
//	m_pInputDevice->SetUp_DeviceStates();

	/* Object */
	m_pLevelManager->Tick();
	m_pObjectManager->Tick();
	m_pComponentManager->Tick();

	m_pLevelManager->Late_Tick();
	//m_pObjectManager->Late_Tick();
	m_pComponentManager->Late_Tick();

	/* ViewProj */
	m_pCameraManager->Make_ViewProj();

	/* Frustum */
	//MGR(CZFrustum)->Make_Plane(m_pCameraManager->Get_ViewProj());

	/* Other Events */
	//m_pPickingManager->Execute_Picking();
	m_pCollisionManager->Tick();



	return S_OK;
}

HRESULT CGameInstance::Render_Engine()
{
	if (FAILED(m_pRenderManager->Render()))
		return E_FAIL;

	m_pTimeManager->Render();

	return S_OK;
}

HRESULT CGameInstance::Initialize()
{
#define SAFE_GET_INSTANCE(name) if (!(name = name->Get_Instance())) return E_FAIL;

	SAFE_GET_INSTANCE(m_pKeyManager);
	SAFE_GET_INSTANCE(m_pObjectManager);
	SAFE_GET_INSTANCE(m_pLevelManager);
	SAFE_GET_INSTANCE(m_pEventManager);
	SAFE_GET_INSTANCE(m_pCollisionManager);
	SAFE_GET_INSTANCE(m_pCameraManager);
	SAFE_GET_INSTANCE(m_pRenderManager);
	SAFE_GET_INSTANCE(m_pTimeManager);
	SAFE_GET_INSTANCE(m_pComponentManager);
	SAFE_GET_INSTANCE(m_pPickingManager);
	SAFE_GET_INSTANCE(m_pGraphicDevice);
	SAFE_GET_INSTANCE(m_pShaderManager);
	SAFE_GET_INSTANCE(m_pSoundDevice);
	SAFE_GET_INSTANCE(m_pInputDevice);

	return S_OK;
}

void CGameInstance::Release()
{
	m_pKeyManager->Destroy_Instance();
	m_pObjectManager->Destroy_Instance();
	m_pLevelManager->Destroy_Instance();
	m_pEventManager->Destroy_Instance();
	m_pCollisionManager->Destroy_Instance();
	m_pGraphicDevice->Destroy_Instance();
	m_pCameraManager->Destroy_Instance();
	m_pRenderManager->Destroy_Instance();
	m_pTimeManager->Destroy_Instance();
	m_pComponentManager->Destroy_Instance();
	m_pPickingManager->Destroy_Instance();
	m_pPrototypeManager->Destroy_Instance();
	m_pShaderManager->Destroy_Instance();
	m_pInputDevice->Destroy_Instance();
	m_pSoundDevice->Destroy_Instance();
}

HRESULT CGameInstance::Clear_BackBuffer_View(_float4 vClearColor)
{
	return m_pGraphicDevice->Clear_BackBuffer_View(vClearColor);
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{
	return m_pGraphicDevice->Clear_DepthStencil_View();
}

HRESULT CGameInstance::Present()
{
	return m_pGraphicDevice->Present();
}

_long CGameInstance::Get_DIMouseMoveState(MOUSEMOVE eMouseMove)
{
	return m_pInputDevice->Get_DIMouseMoveState(eMouseMove);
}

void CGameInstance::Play_Sound(const _tchar* strSoundKey, CHANNEL_GROUP iGroupIndex, _float fVolumeRatio)
{
	wstring strKey = strSoundKey;
	strKey += L".wav";

	m_pSoundDevice->Play_Sound(strKey.c_str(), iGroupIndex, fVolumeRatio);
}

void CGameInstance::Play_Sound_Rand(const _tchar* strSoundKey, const _uint& iRandCnt, CHANNEL_GROUP iGroupIndex, _float fVolumeRatio)
{
	wstring strKey = strSoundKey;

	_int iRand = random(0, iRandCnt);
	_tchar szTemp[MIN_STR] = {};

	_itow_s(iRand, szTemp, 10);

	strKey += szTemp;
	strKey += L".wav";

	m_pSoundDevice->Play_Sound(strKey.c_str(), iGroupIndex, fVolumeRatio);
}

void CGameInstance::Play_BGM(const _tchar* strSoundKey)
{
	wstring strKey = strSoundKey;
	strKey += L".wav";
	m_pSoundDevice->Play_BGM(strKey.c_str());
}

void CGameInstance::Stop_Sound(CHANNEL_GROUP eType)
{
	m_pSoundDevice->Stop_Sound(eType);

}

void CGameInstance::Stop_All()
{
	m_pSoundDevice->Stop_All();
}

void CGameInstance::Set_Volume(_float fVolume)
{
	m_pSoundDevice->Set_Volume(fVolume);
}

void CGameInstance::Set_ChannelVolume(CHANNEL_GROUP  eID, _float fVolume)
{
	m_pSoundDevice->Set_ChannelVolume(eID, fVolume);
}

void CGameInstance::Set_ChannelVolume(CHANNEL_GROUP eID, const _uint& iChannelIndex, _float fVolume)
{
	m_pSoundDevice->Set_ChannelVolume(eID, iChannelIndex, fVolume);
}

_double CGameInstance::Get_DT()
{
	return m_pTimeManager->Get_DT();
}

KEY_STATE CGameInstance::Get_KeyState(KEY _key)
{
	return m_pKeyManager->Get_KeyState(_key);
}

CGameObject* CGameInstance::Get_StaticObj(const _uint& iKeyValue)
{
	return m_pObjectManager->Get_StaticObj(iKeyValue);
}

list<CGameObject*>& CGameInstance::Get_ObjGroup(const _uint& iGroupIdx)
{
	return m_pObjectManager->Get_ObjGroup(iGroupIdx);
}

void CGameInstance::Delete_Objects(const _uint& iGroupIdx)
{
	return m_pObjectManager->Delete_Objects(iGroupIdx);
}

void CGameInstance::Add_Camera(wstring strKey, CCamera * pCamera)
{
	m_pCameraManager->Add_Camera(strKey, pCamera);
}

CCamera * CGameInstance::Change_Camera(wstring strKey)
{
	return m_pCameraManager->Change_Camera(strKey);
}

CCamera* CGameInstance::Get_CurCam()
{
	return m_pCameraManager->Get_CurCam();
}

_float4 CGameInstance::Get_ViewPos()
{
	return m_pCameraManager->Get_ViewPos();
}

void CGameInstance::Delete_GameObject(CGameObject * pGameObject)
{
	m_pEventManager->Delete_GameObject(pGameObject);
}

void CGameInstance::Create_GameObject(CGameObject* pGameObject, const _uint& iGroupIdx)
{
	m_pEventManager->Create_GameObject(pGameObject, iGroupIdx);

}

void CGameInstance::Delete_Component(CComponent* pComponent, CGameObject* pGameObject)
{
	m_pEventManager->Delete_Component(pComponent, pGameObject);

}

void CGameInstance::Create_Component(CComponent* pComponent, CGameObject* pGameObject)
{
	m_pEventManager->Create_Component(pComponent, pGameObject);
}

void CGameInstance::Enable_GameObject(CGameObject * pGameObject)
{
	m_pEventManager->Enable_GameObject(pGameObject);
}

void CGameInstance::Disable_GameObject(CGameObject * pGameObject)
{
	m_pEventManager->Disable_GameObject(pGameObject);
}

void CGameInstance::Disable_Component(CComponent* pComponent)
{
	m_pEventManager->Disable_Component(pComponent);

}

void CGameInstance::Enable_Component(CComponent* pComponent)
{
	m_pEventManager->Enable_Component(pComponent);

}

void CGameInstance::Create_StaticObject(CGameObject * pGameObject, const _uint& iObjectID)
{
	m_pEventManager->Create_StaticObject(pGameObject, iObjectID);
}

void CGameInstance::Change_Level(CLevel* pLevel)
{
	m_pEventManager->Change_Level(pLevel);
}

CGameObject* CGameInstance::Clone_GameObject(const _uint& _iID)
{
	return m_pPrototypeManager->Clone_GameObject(_iID);
}

CComponent* CGameInstance::Clone_Component(const _uint& _iID)
{
	return m_pPrototypeManager->Clone_Component(_iID);
}

HRESULT CGameInstance::Add_GameObject_Prototype(const _uint& _iID, CGameObject* pGameObject)
{
	return m_pPrototypeManager->Add_GameObject_Prototype(_iID, pGameObject);
}

HRESULT CGameInstance::Add_Component_Prototype(const _uint& _iID, CComponent* pComponent)
{
	return m_pPrototypeManager->Add_Component_Prototype(_iID, pComponent);
}
