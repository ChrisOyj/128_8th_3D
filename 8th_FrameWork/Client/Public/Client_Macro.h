#pragma once

#define MOUSE_DPI_X		0.015f
#define MOUSE_DPI_Y		0.015f

#define PI				3.141592f
#define PLAYER			CUser::Get_Instance()->Get_Player()

#define	DEFAULT_TRANS_MATRIX			XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(90.0f))* XMMatrixRotationZ(XMConvertToRadians(180.0f))

#define GET_STATE(enumname, classname) static_cast<classname*>(CState_Manager::Get_Instance()->Get_State(enumname))

#define COLOR_RED		_float4(0.8f, 0.2f, 0.2f, 1.0f)
#define COLOR_YELLOW	_float4(1.f, 1.f, 0.6f, 1.0f)
#define COLOR_ORANGE	_float4(0.8f, 0.4f, 0.2f, 1.0f)
#define COLOR_GRAY		_float4(0.5f, 0.5f, 0.5f, 1.0f)
#define COLOR_WHITE		_float4(0.9f, 0.9f, 0.9f, 1.0f)
#define COLOR_BLACK		_float4(0.1f, 0.1f, 0.1f, 1.0f)
#define COLOR_BROWN		_float4(0.4f, 0.2f, 0.1f, 1.0f)

/* Glow*/
#define	GLOW_CHAKRA(power)		_float4(0.f, 0.5f, 1.f, power)


#define	DECLARE_STATE(classname) public:\
classname* Clone() {return new classname(*this);}

#define SHADER_BINDING(classname, variablename) GET_COMPONENT(CShader)->CallBack_SetRawValues += bind(&classname::Set_ShaderResource, this, placeholders::_1, variablename)
#define SHADER_BINDINGOUT(classname, variablename) GET_COMPONENT(CShader)->CallBack_SetRawValues += bind(&classname::Set_ShaderResource, this, placeholders::_1, variablename)
#define CAMERA_LERP(enumname) CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::enumname)

#define	DECLARE_LEVEL(classname) public:\
static classname* Create() \
{ \
classname* pInstance = new classname(); \
if (FAILED(pInstance->Initialize()))\
{\
	string text = "Failed to Initialize_Prototype : ";\
	text += typeid(classname).name();\
	wstring wText;\
	wText.assign(text.begin(), text.end());\
	Call_MsgBox(wText.c_str());\
	delete pInstance;\
	pInstance = nullptr;\
}\
return pInstance;\
}\

#define	DECLARE_GAMEOBJECT(classname) public:\
static classname* Create() \
{ \
classname* pInstance = new classname(); \
if (FAILED(pInstance->Initialize_Prototype()))\
{\
	string text = "Failed to Initialize_Prototype : ";\
	text += typeid(classname).name();\
	wstring wText;\
	wText.assign(text.begin(), text.end());\
	Call_MsgBox(wText.c_str());\
	delete pInstance;\
	pInstance = nullptr;\
}\
return pInstance;\
}\



