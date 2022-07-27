#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_DLL _declspec(dllexport)
#else
#define ENGINE_DLL _declspec(dllimport)
#endif

// =========My Macro=========

#define	GAMEINSTANCE CGameInstance::Get_Instance()

#define DEVICE	CGameInstance::Get_Instance()->Get_Device()

#define	HASHCODE(type)	typeid(type).hash_code()

#define DECLARE_PROTOTYPE(type)		public:																\
									virtual type* Clone() {return new type(*this);}						\
									friend class CPrototype_Manager;									\
									private:															\
									static type*	Create_Prototype()									\
									{																	\
										type*	pObj = new type;										\
										if (FAILED(pObj->Initialize_Prototype()))						\
										{																\
											Call_MsgBox(L"Failed to Initialize_Prototype");					\
											return nullptr;												\
										}																\
										return pObj;													\
									}

#define MATRIX_NOSCALE				0x001
#define MATRIX_NOTURN				0x002
#define MATRIX_IDENTITY			    0x004
#define MARTIX_NOTRANS				0x008

#define MATRIX_DEFAULT				0x00f

#define CLONE_GAMEOBJECT(type)				CPrototype_Manager::Get_Instance()->Clone_GameObject<type>()
#define CLONE_COMPONENT(type)				CPrototype_Manager::Get_Instance()->Clone_Component<type>()


// ==========================

#define DT	CGameInstance::Get_Instance()->Get_DT()
#define fDT	(float)CGameInstance::Get_Instance()->Get_DT()

#define KEY(key, state) CGameInstance::Get_Instance()->Get_KeyState(KEY::key) == KEY_STATE::state
#define MOUSE_MOVE(mousemove) CGameInstance::Get_Instance()->Get_DIMouseMoveState(mousemove)

/* Event_Manager */
#define CREATE_GAMEOBJECT(GameObject) CGameInstance::Get_Instance()->Create_GameObject(GameObject)
#define DELETE_GAMEOBJECT(GameObject) CGameInstance::Get_Instance()->Delete_GameObject(GameObject)
#define ENABLE_GAMEOBJECT(GameObject) CGameInstance::Get_Instance()->Enable_GameObject(GameObject)
#define DISABLE_GAMEOBJECT(GameObject) CGameInstance::Get_Instance()->Disable_GameObject(GameObject)
#define CREATE_STATIC(GameObject, hashcode) CGameInstance::Get_Instance()->Create_StaticObject(GameObject, hashcode)
#define CHANGE_LEVEL(iLevelID, pLevel) CGameInstance::Get_Instance()->Change_Level(iLevelID, pLevel)
#define CREATE_COMPONENT(Component) CGameInstance::Get_Instance()->Create_Component(Component)
#define DELETE_COMPONENT(Component) CGameInstance::Get_Instance()->Delete_Component(Component)
#define ENABLE_COMPONENT(Component) CGameInstance::Get_Instance()->Enable_Component(Component)
#define DISABLE_COMPONENT(Component) CGameInstance::Get_Instance()->Disable_Component(Component)

#define		MIN_STR			64
#define		MAX_STR			256

#define		MAX_VERTEX_ELEMENT			65

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
