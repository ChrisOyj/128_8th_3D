#pragma once

#pragma warning (disable : 4005)
#pragma warning (disable : 4251)
#pragma warning (disable : 4273)
#pragma warning (disable : 26812)
#pragma warning (disable : 4049)
#pragma warning (disable : 4217)

#include <d3d11.h>
#include <DirectXMath.h>
#include <dxgidebug.h>

#include <Effects11/d3dx11effect.h>
#include <DirectXTK/DDSTextureLoader.h>
#include <DirectXTK/WICTextureLoader.h>
#include <DirectXTK/SpriteBatch.h>
#include <DirectXTK/SpriteFont.h>
#include <DirectXTK/ScreenGrab.h>
#include <DirectXTex/DirectXTex.h>

#include <DirectXCollision.h>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>


#include <d3dcompiler.h>

using namespace DirectX;

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>

#include <process.h>

#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <random>
#include <typeinfo>
#include <functional>

#include <io.h>
#include "fmod/fmod.h"
#include "fmod/fmod.hpp"

#include <wrl.h>
using namespace Microsoft::WRL;
using namespace std;

#include <Delegate/Delegate.h>

#include "Engine_Enum.h"
#include "Engine_Typedef.h"
#include "Engine_Function.h"
#include "Engine_Functor.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"


#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
//#define new DBG_NEW 

#endif

#endif // _DEBUG

using namespace Engine;




