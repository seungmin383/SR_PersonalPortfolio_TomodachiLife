#ifndef Engine_Define_h__
#define Engine_Define_h__

#include <d3d11.h>
#include <DirectXMath.h>

#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <functional>
#include <thread>
#include <future>
#include <string>
#include <unordered_map>
#include <ctime>

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
