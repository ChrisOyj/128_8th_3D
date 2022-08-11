#pragma once
#include "Engine_Defines.h"
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
#include "CShader_Manager.h"
#include "Font_Manager.h"

//매니져 매니저..

BEGIN(Engine)

class CCamera;
class CGameObject;
class CComponent;
class CLevel;
enum class KEY;
enum class KEY_STATE;

class ENGINE_DLL CGameInstance
{
	DECLARE_SINGLETON(CGameInstance)

private:
	CGameInstance();
	~CGameInstance();

public: /*Get, Set*/
	ComPtr<ID3D11Device>		Get_Device();
	ComPtr<ID3D11DeviceContext>	Get_DeviceContext();

public:
	HRESULT Initialize_Engine(const GRAPHICDESC& GraphicDesc, const SOUNDDESC& SoundDesc);
	HRESULT Tick_Engine();
	HRESULT Render_Engine();
	void	Release();

public: /* For. Graphic_Device */
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT Clear_DepthStencil_View();
	HRESULT Present();

public: /* For. Input_Device */
	_long	Get_DIMouseMoveState(MOUSEMOVE eMouseMove);

public: /* For. Sound_Device */
	void		Play_Sound(const _tchar* strSoundKey, CHANNEL_GROUP iGroupIndex, _float fVolumeRatio = 1.f);
	void		Play_Sound_Rand(const _tchar* strSoundKey, const _uint& iRandCnt, CHANNEL_GROUP iGroupIndex, _float fVolumeRatio = 1.f);
	void		Play_BGM(const _tchar* strSoundKey);

	void		Stop_Sound(CHANNEL_GROUP eType);
	void		Stop_All();

	void		Set_Volume(_float fVolume);
	void		Set_ChannelVolume(CHANNEL_GROUP eID, _float fVolume);
	void		Set_ChannelVolume(CHANNEL_GROUP eID, const _uint& iChannelIndex, _float fVolume);

public: /* For. Time_Manager */
	_double		Get_DT();
	_double		Get_FPSLimitTime();
	void		Set_FPSLimitTIme(_double dTime);
	_bool		Can_Update();

public: /* For. Key_Manager */
	KEY_STATE	Get_KeyState(KEY _key);
	vector<CKey_Manager::tKeyInfo>& Get_KeyList();

public:	/* For. Object_Manager */
	CGameObject*			Get_StaticObj(const _uint& iKeyValue);
	list<CGameObject*>&		Get_ObjGroup(const _uint& iGroupIdx);
	void					Delete_Objects(const _uint& iGroupIdx);

public:
	void					Clear_All_Components();



public: /* For. Camera_Manager */
	void		Add_Camera(wstring strKey, CCamera* pCamera);
	CCamera*	Change_Camera(wstring strKey);
	CCamera*	Get_CurCam();
	_float4		Get_ViewPos();

public: /* For. Event_Manager */
	void	Delete_GameObject(CGameObject* pGameObject);
	void	Create_GameObject(CGameObject* pGameObject, const _uint& iGroupIdx);

	void	Delete_Component(CComponent* pComponent, CGameObject* pGameObject);
	void	Create_Component(CComponent* pComponent, CGameObject* pGameObject);

	void	Enable_GameObject(CGameObject* pGameObject);
	void	Disable_GameObject(CGameObject* pGameObject);

	void	Disable_Component(CComponent* pComponent);
	void	Enable_Component(CComponent* pComponent);

	void	Create_StaticObject(CGameObject* pGameObject, const _uint& iObjectID);
	void	Change_Level(CLevel* pLevel);

	void	Clear_All_Event();


public: /* For. Prototype_Manager */
	CGameObject* Clone_GameObject(const _uint& _iID);
	CComponent* Clone_Component(const _uint& _iID);

	HRESULT Add_GameObject_Prototype(const _uint& _iID, CGameObject* pGameObject);
	HRESULT Add_Component_Prototype(const _uint& _iID, CComponent* pComponent);

	void	Delete_GameObject_Prototypes();
	void	Delete_Component_Prototypes();

	CGameObject* Find_GameObject_Prototype(const _uint& _iID);
	CComponent* Find_Component_Prototype(const _uint& _iID);

public: /* For. Font_Manager */
	HRESULT Add_Font(const _tchar* pFontTag, const _tchar* pFontFilePath);
	HRESULT Render_Font(const _tchar* pFontTag, const _tchar* pString, const _float2& vPosition, const _float4& vColor);

public: /* For. Shader_Manager */
	HRESULT		Load_EffectFile(const _tchar* pFilePath);


private:
	CEvent_Manager* m_pEventManager = nullptr;
	CLevel_Manager* m_pLevelManager = nullptr;
	CObject_Manager* m_pObjectManager = nullptr;
	CCamera_Manager* m_pCameraManager = nullptr;
	CCollision_Manager* m_pCollisionManager = nullptr;
	CTime_Manager* m_pTimeManager = nullptr;
	CKey_Manager* m_pKeyManager = nullptr;
	CRender_Manager* m_pRenderManager = nullptr;
	CComponent_Manager* m_pComponentManager = nullptr;
	CPicking_Manager* m_pPickingManager = nullptr;
	CPrototype_Manager* m_pPrototypeManager = nullptr;
	CShader_Manager* m_pShaderManager = nullptr;
	CGraphic_Device* m_pGraphicDevice = nullptr;
	CInput_Device* m_pInputDevice = nullptr;
	CSound_Device* m_pSoundDevice = nullptr;
	CFont_Manager* m_pFontManager = nullptr;

private:
	HRESULT	Initialize();
};

END