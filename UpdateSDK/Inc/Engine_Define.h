#ifndef Engine_Define_h__
#define Engine_Define_h__

#include <d3d11.h>
#include <DirectXMath.h>
#include<DirectXCollision.h>

#include "fx11/d3dx11effect.h"

#include <DirectXTK/DDSTextureLoader.h>	//dds 로드
#include <DirectXTK/WICTextureLoader.h>	//window tex 로드

#include <DirectXTK/ScreenGrab.h>
#include <DirectXTK/PrimitiveBatch.h>
#include <DirectXTK/Effects.h>		// 쉐이더
#include <DirectXTK/VertexTypes.h>	// 정점

#include <DirectXTK/SpriteBatch.h>
#include <DirectXTK/SpriteFont.h>

// 나중에 절때 뺼 것
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
// 

#include <d3dcompiler.h>

#include <json/json.hpp>

#include <vector>
#include <list>
#include <map>
#include <stack>
#include <algorithm>
#include <functional>
#include <thread>
#include <future>
#include <string>
#include <unordered_map>
#include <ctime>
#include <fstream>
#include <iostream>
#include <filesystem>

//#include "fmod.h"
//#include "fmod.hpp"
//#pragma comment(lib, "fmod_vc.lib")
//#include <io.h>

#include <cstdlib>

using namespace std;
using namespace DirectX;

#include "Engine_Enum.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_Typedef.h"
#include "Engine_Function.h"


#define DIRECTINPUT_VERSION	0x0800
#include <dinput.h>

#pragma warning(disable : 4251)
#pragma warning(disable : 4099)

namespace Engine
{
	static const unsigned int g_iMaxNumBones = 128;
}
#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif

using namespace Engine;

#endif // Engine_Define_h__
