#pragma once
#include "Engine_Defines.h"

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

public: /* For. Key_Manager */
	KEY_STATE	Get_KeyState(KEY _key);

public:	/* For. Object_Manager */
	CGameObject*			Get_StaticObj(const _uint& iKeyValue);
	list<CGameObject*>&		Get_ObjGroup(const _uint& iGroupIdx);
	void					Delete_Objects(const _uint& iGroupIdx);



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

public: /* For. Prototype_Manager */
	CGameObject* Clone_GameObject(const _uint& _iID);
	CComponent* Clone_Component(const _uint& _iID);

	HRESULT Add_GameObject_Prototype(const _uint& _iID, CGameObject* pGameObject);
	HRESULT Add_Component_Prototype(const _uint& _iID, CComponent* pComponent);

private:
	HRESULT	Initialize();
};

END