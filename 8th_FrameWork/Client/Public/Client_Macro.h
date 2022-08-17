#pragma once

#define MOUSE_DPI_X		0.03f
#define MOUSE_DPI_Y		0.03f

#define PI				3.141592f


#define COLOR_RED		_float4(0.8f, 0.2f, 0.2f, 1.0f)
#define COLOR_YELLOW	_float4(1.f, 1.f, 0.6f, 1.0f)
#define COLOR_ORANGE	_float4(0.8f, 0.4f, 0.2f, 1.0f)
#define COLOR_GRAY		_float4(0.5f, 0.5f, 0.5f, 1.0f)
#define COLOR_WHITE		_float4(0.9f, 0.9f, 0.9f, 1.0f)
#define COLOR_BLACK		_float4(0.1f, 0.1f, 0.1f, 1.0f)
#define COLOR_BROWN		_float4(0.4f, 0.2f, 0.1f, 1.0f)


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



