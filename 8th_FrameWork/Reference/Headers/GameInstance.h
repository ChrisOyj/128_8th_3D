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
#include "CResource_Manager.h"
#include "CFrustum_Manager.h"
#include "CTarget_Manager.h"
#include "CLight_Manager.h"

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
	HRESULT		Load_SoundFile(wstring wstrFolderPath);


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
	_float	Get_RealFDT();
	void	Set_TimeSpeed(_float fSpeed);

public: /* For. Key_Manager */
	KEY_STATE	Get_KeyState(KEY _key);
	vector<CKey_Manager::tKeyInfo>& Get_KeyList();

public: /* For. Collision_Manager */
	HRESULT Check_Group(const _uint& _eLeft, const _uint& _eRight);


public:	/* For. Render_Manager */
	void	Bake_StaticShadow(vector <CGameObject*>& MapList, _float fDistance);

public:	/* For. Object_Manager */
	template <typename T>
	T* Get_StaticObj()
	{
		return m_pObjectManager->Get_StaticObj<T>();
	}
	list<CGameObject*>& Get_ObjGroup(const _uint& iGroupIdx);
	void					Delete_Objects(const _uint& iGroupIdx);

public:
	void					Clear_All_Components();

public: /* For Picking_Manager */
	void	Compute_WorldRay();
	void	Regist_Mesh(class CMesh* pMesh, _float fDistanceToPlayer);
	_bool	Is_Picked(CMesh* pRenderer, _float4* pOut, _float4* pOutNormal = nullptr);



public: /* For. Camera_Manager */
	void		Add_Camera(wstring strKey, CCamera* pCamera);
	CCamera* Change_Camera(wstring strKey);
	CCamera* Get_CurCam();
	_float4		Get_ViewPos();
	_float4x4	Get_CurProjMatrix();
	_float4x4	Get_CurViewMatrix();
	CCamera* Find_Camera(wstring strKey);


public: /* For. Event_Manager */
	void	Delete_GameObject(CGameObject* pGameObject);
	void	Create_GameObject(CGameObject* pGameObject, const _uint& iGroupIdx);

	void	Delete_Component(CComponent* pComponent, CGameObject* pGameObject);
	void	Create_Component(CComponent* pComponent, CGameObject* pGameObject);

	void	Enable_GameObject(CGameObject* pGameObject);
	void	Disable_GameObject(CGameObject* pGameObject);

	void	Disable_Component(CComponent* pComponent);
	void	Enable_Component(CComponent* pComponent);

	void	Create_StaticObject(CGameObject* pGameObject, _hashcode hcClassName);
	void	Change_Level(CLevel* pLevel);

	void	Clear_All_Event();
	void	Clear_Enable_Events();


public: /* For. Prototype_Manager */
	CGameObject* Clone_GameObject(_hashcode	hcClassName);
	CComponent* Clone_Component(_hashcode	hcClassName);
	HRESULT	Add_GameObject_Prototype(CGameObject* pGameObject, _hashcode _hcCode);


	template <typename T>
	T* Clone_GameObject()
	{
		return m_pPrototypeManager->Clone_GameObject<T>();
	}

	template <typename T>
	HRESULT	Add_GameObject_Prototype(T* pGameObject)
	{
		return m_pPrototypeManager->Add_GameObject_Prototype<T>(pGameObject);
	}

	template <typename T>
	T* Find_GameObject_Prototype()
	{
		return m_pPrototypeManager->Find_GameObject_Prototype<T>();
	}

	template <typename T>
	T* Clone_Component()
	{
		return m_pPrototypeManager->Clone_Component<T>();
	}

	template <typename T>
	HRESULT	Add_Component_Prototype(T* pComponent)
	{
		return m_pPrototypeManager->Add_Component_Prototype<T>(pComponent);
	}

	template <typename T>
	T* Find_Component_Prototype()
	{
		return m_pPrototypeManager->Find_Component_Prototype<T>();
	}

	void	Delete_GameObject_Prototypes();
	void	Delete_Component_Prototypes();

public: /* For. Font_Manager */
	HRESULT Add_Font(const _tchar* pFontTag, const _tchar* pFontFilePath);
	HRESULT Render_Font(const _tchar* pFontTag, const _tchar* pString, const _float2& vPosition, const _float4& vColor);

public: /* For. Shader_Manager */
	HRESULT		Load_EffectFile(const _tchar* pFilePath);
	HRESULT		Set_RawValue(const _uint& iIndex, const char* pConstantName, void* pData, _uint iDataSize);
	HRESULT		Set_RawValue_All(const char* pConstantName, void* pData, _uint iDataSize);

public:/* For. Resource_Manager */
	MODEL_DATA* Get_ModelData(wstring wstrFilePath, MODEL_TYPE eType);
	CResource* Get_Resource(wstring wstrResourceKey);

public:/* For. Frustum_Manager */
	_bool isIn_Frustum_InWorldSpace(_vector vWorldPoint, _float fRange = 0.f);
	_bool isIn_Frustum_InLocalSpace(_vector vLocalPoint, _float fRange = 0.f);
	void Transform_ToLocalSpace(_matrix WorldMatrixInv);

public: /*For Light_Manager */
	HRESULT Add_Light(const LIGHTDESC& LightDesc);
	void Pop_Light();
	HRESULT Load_Lights(wstring wstrPath);
	const LIGHTDESC* Get_LightDesc(_uint iIndex);
	void	Clear_Lights();

public: /* For.Target_Manager */
	ComPtr<ID3D11ShaderResourceView> Get_RenderTarget_SRV(wstring pTargetTag);
	class CShader* Get_DeferredShader();

private:
	CGraphic_Device* m_pGraphicDevice = nullptr;
	CInput_Device* m_pInputDevice = nullptr;
	CSound_Device* m_pSoundDevice = nullptr;

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
	CResource_Manager* m_pResourceManager = nullptr;
	CFont_Manager* m_pFontManager = nullptr;
	CFrustum_Manager* m_pFrustumManager = nullptr;
	CTarget_Manager* m_pTargetManager = nullptr;
	CLight_Manager* m_pLightManager = nullptr;

private:
	HRESULT	Initialize();
};

END