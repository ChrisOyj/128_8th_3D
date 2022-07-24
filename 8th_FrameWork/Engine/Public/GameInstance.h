#pragma once
#include "Graphic_Device.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "Render_Manager.h"
#include "Event_Manager.h"
#include "Time_Manager.h"
#include "Key_Manager.h"
#include "Camera_Manager.h"
#include "Collision_Manager.h"
#include "Shader_Manager.h"
#include "ZFrustum.h"
#include "Sound_Device.h"
//매니져 매니저..

BEGIN(Engine)

class ENGINE_DLL CGameInstance final
{
	DECLARE_SINGLETON(CGameInstance)

private:
	CGameInstance();
	~CGameInstance();

public: /*Get, Set*/
	LPDIRECT3DDEVICE9			Get_Device();
	LPD3DXSPRITE				Get_Sprite(void);
	LPD3DXFONT					Get_Font(void);

public:
	HRESULT Initialize_Engine(const GRAPHICDESC& GraphicDesc);
	HRESULT Tick_Engine();
	HRESULT Render_Engine();
	void	Release();

public: /* For. Graphic_Device */	
	void		Render_Begin(void);
	void		Render_End(void);

public: /* For. Input_Device */
	_long	Get_DIMouseMoveState(MOUSEMOVE eMouseMove);

public: /* For. Sound_Device */
	void		Play_Sound(const _tchar* strSoundKey, _float fVolume = 1.f);
	void		Play_Sound_Rand(const _tchar* strSoundKey, const _uint& iRandNum, _float fVolume = 1.f);
	void		Play_Sound_Player(const _tchar* strSoundKey, _float fVolume = 1.f);
	void		Play_Sound_Player_Rand(const _tchar* strSoundKey, const _uint& iRandNum, _float fVolume = 1.f);
	void		PlayBGM(const _tchar* strSoundKey, _float fVolume);
	void		StopSound(CHANNELID eType);
	void		StopAll();
	void		SetVolume(_float fVolume);
	void		SetChannelVolume(CHANNELID eID, _float fVolume);


public: /* For. Time_Manager */
	_double		Get_DT(bool bReal = false);

public: /* For. Key_Manager */
	KEY_STATE	Get_KeyState(KEY _key);

public: /* For. Camera_Manager */
	void		Add_Camera(wstring strKey, CCamera* pCamera);
	CCamera*	Change_Camera(wstring strKey);

public: /* For. Event_Manager */
	void	Delete_GameObject(CGameObject*	pGameObject);
	void	Create_GameObject(CGameObject*	pGameObject);
	void	Enable_GameObject(CGameObject*	pGameObject);
	void	Disable_GameObject(CGameObject*	pGameObject);
	void	Create_StaticObject(CGameObject* pGameObject, _hashcode hashcode);
	void	Change_Level(_uint iLevelID, CLevel * pLevel);

public: /* For. Shader_Manager */
	HRESULT		Set_Parameter(const CHAR * parameterName, const _float* pFloats, _uint iNum);
	HRESULT		Set_Texture(LPDIRECT3DBASETEXTURE9	pTexture);
	HRESULT		Set_float4x4(const CHAR * parameterName, const _float4x4& _float4x4);

public: /* For. Picking_Manager */
	void	Add_PickingObject(CGameObject* pGameObject, _float fDistanceToPlayer);
	void	Add_PickingUI(class CUI* pUI);
	void	Execute_Picking();
	void	SetRayCenterAnyTime(_bool bBool);
	_bool	GetRayCenterAnyTime();

	/*ShotGun*/
	void	Execute_RandomPicking();
	_bool	IsOnMouseRay(CGameObject* pGameObject);
	_bool	Is3DPicked(CGameObject* pGameObject, _float3*	pOut, _float3* pOutNormal = nullptr);


public: /* For. ZFrustum */
	_bool	IsInPlane(const _float3& vPos);
	_bool	IsInSphere(const _float3& vPos, const _float& fRadius);



private:
	HRESULT	Initialize();
};

END