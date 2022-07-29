#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_DLL _declspec(dllexport)
#else
#define ENGINE_DLL _declspec(dllimport)
#endif

#define	SAFE_DELETE(Instance) if (Instance)\
		{\
		delete Instance;\
		Instance = nullptr;\
		}\

// =========My Macro=========

#define	GAMEINSTANCE CGameInstance::Get_Instance()

#define DEVICE	CGameInstance::Get_Instance()->Get_Device().Get()
#define DEVICE_CONTEXT	CGameInstance::Get_Instance()->Get_DeviceContext().Get()

#define	HASHCODE(type)	typeid(type).hash_code()

#define DECLARE_PROTOTYPE(type)		public:																\
									virtual type* Clone() {return new type(*this);}						\
									friend class CPrototype_Manager;									\

#define MATRIX_NOSCALE				0x001
#define MATRIX_NOTURN				0x002
#define MATRIX_TYPEENTITY			    0x004
#define MARTIX_NOTRANS				0x008

#define MATRIX_DEFAULT				0x00f

#define CLONE_GAMEOBJECT(ID)				CGameInstance::Get_Instance()->Clone_GameObject(ID)
#define CLONE_COMPONENT(ID)					CGameInstance::Get_Instance()->Clone_Component(ID)


// ==========================

#define DT	CGameInstance::Get_Instance()->Get_DT()
#define fDT	(float)CGameInstance::Get_Instance()->Get_DT()

#define KEY(key, state) CGameInstance::Get_Instance()->Get_KeyState(KEY::key) == KEY_STATE::state
#define MOUSE_MOVE(mousemove) CGameInstance::Get_Instance()->Get_DIMouseMoveState(mousemove)

/* Event_Manager */
#define CREATE_GAMEOBJECT(GameObject, GroupIndex) CGameInstance::Get_Instance()->Create_GameObject(GameObject, GroupIndex)
#define DELETE_GAMEOBJECT(GameObject) CGameInstance::Get_Instance()->Delete_GameObject(GameObject)
#define ENABLE_GAMEOBJECT(GameObject) CGameInstance::Get_Instance()->Enable_GameObject(GameObject)
#define DISABLE_GAMEOBJECT(GameObject) CGameInstance::Get_Instance()->Disable_GameObject(GameObject)
#define CREATE_STATIC(GameObject, hashcode) CGameInstance::Get_Instance()->Create_StaticObject(GameObject, hashcode)
#define CHANGE_LEVEL(iLevelID, pLevel) CGameInstance::Get_Instance()->Change_Level(iLevelID, pLevel)
#define CREATE_COMPONENT(Component, GameObject) CGameInstance::Get_Instance()->Create_Component(Component, GameObject)
#define DELETE_COMPONENT(Component, GameObject) CGameInstance::Get_Instance()->Delete_Component(Component, GameObject)
#define ENABLE_COMPONENT(Component) CGameInstance::Get_Instance()->Enable_Component(Component)
#define DISABLE_COMPONENT(Component) CGameInstance::Get_Instance()->Disable_Component(Component)

#define		MIN_STR			64
#define		MAX_STR			256

#define		ToRadian(fAngle)	XMConvertToRadians(fAngle)

#define		MAX_VERTEX_ELEMENT			65

// ====================== Shader Flag ============================

#define		SH_DEFAULT		0
#define		SH_BLUR			0x001
#define		SH_CARTOON		0x002
#define		SH_OUTLINE		0x004
#define		SH_GLOW			0x008

#define BEGIN(NAMESPACE) namespace NAMESPACE { 
#define END }


#define NO_COPY(ClassName)								\
ClassName(const ClassName&) = delete;					\
ClassName& operator=(const ClassName&) = delete;

#define DECLARE_SINGLETON(type) public:								\
									NO_COPY(type)					\
									static type* Get_Instance() {	\
										static type mgr;			\
										return &mgr;				\
									}
