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

#define	SAFE_DESTROY(Instance) if (Instance)\
		{\
		Instance->Destroy_Instance();\
		Instance = nullptr;\
		}\

// =========My Macro=========

#define	GAMEINSTANCE CGameInstance::Get_Instance()

#define DEVICE	CGameInstance::Get_Instance()->Get_Device()
#define DEVICE_CONTEXT	CGameInstance::Get_Instance()->Get_DeviceContext()
#define PDEVICE	CGameInstance::Get_Instance()->Get_Device().Get()
#define PDEVICE_CONTEXT	CGameInstance::Get_Instance()->Get_DeviceContext().Get()

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
#define CREATE_STATIC(GameObject, keyID) CGameInstance::Get_Instance()->Create_StaticObject(GameObject, keyID)
#define CHANGE_LEVEL(pLevel) CGameInstance::Get_Instance()->Change_Level(pLevel)
#define CREATE_COMPONENT(Component, GameObject) CGameInstance::Get_Instance()->Create_Component(Component, GameObject)
#define DELETE_COMPONENT(Component, GameObject) CGameInstance::Get_Instance()->Delete_Component(Component, GameObject)
#define ENABLE_COMPONENT(Component) CGameInstance::Get_Instance()->Enable_Component(Component)
#define DISABLE_COMPONENT(Component) CGameInstance::Get_Instance()->Disable_Component(Component)

#define		MIN_STR			64
#define		MAX_STR			256

#define		ToRadian(fAngle)	XMConvertToRadians(fAngle)

#define		MAX_VERTEX_ELEMENT			65
#define		ZERO_VECTOR				_float4(0.f, 0.f, 0.f, 1.f)

// ====================== Shader Flag ============================

#define		SH_DEFAULT		0
#define		SH_BLUR			0x001
#define		SH_CARTOON		0x002
#define		SH_OUTLINE		0x004
#define		SH_GLOW			0x008

// ==============================================================

#define BIND_SHADERRESOURCES(classname, variablename) vector<CShader*>	vecShdaer = m_pOwner->Get_Component<CShader>();\
if (vecShdaer.empty())\
return;\
vecShdaer[0]->CallBack_SetRawValues +=\
bind(&classname::Set_ShaderResource, this, placeholders::_1, variablename);\

#define REMOVE_SHADERRESOURCES(classname, variablename) vector<CShader*>	vecShdaer = m_pOwner->Get_Component<CShader>();\
if (vecShdaer.empty())\
return;\
vecShdaer[0]->CallBack_SetRawValues -=\
bind(&classname::Set_ShaderResource, this, placeholders::_1, variablename);\


#define BEGIN(NAMESPACE) namespace NAMESPACE { 
#define END }

#define NO_COPY(ClassName)								\
ClassName(const ClassName&) = delete;					\
ClassName& operator=(const ClassName&) = delete;


#define DECLARE_SINGLETON(ClassName)					\
		NO_COPY(ClassName)								\
public :												\
	static ClassName* Get_Instance();					\
	static void Destroy_Instance();			\
private:												\
	static ClassName* m_pInstance;

#define IMPLEMENT_SINGLETON(ClassName)					\
ClassName* ClassName::m_pInstance = nullptr;			\
ClassName* ClassName::Get_Instance()					\
{														\
	if (nullptr == m_pInstance)							\
		m_pInstance = new ClassName;					\
	return m_pInstance;									\
}														\
void ClassName::Destroy_Instance()						\
{														\
	SAFE_DELETE(m_pInstance);							\
}


//#define DECLARE_SINGLETON(type) public:								\
//									NO_COPY(type)					\
//									static type* Get_Instance() {	\
//										static type mgr;			\
//										return &mgr;				\
//									}								\

									
