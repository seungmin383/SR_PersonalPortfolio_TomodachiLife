#pragma once

#include <Windows.h>   // HWND, HINSTANCE 선언을 보장

#ifdef new
#undef new
#endif

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#define UTF8(string) u8##string

namespace Maptool
{

}
using namespace std;
using namespace ImGui;
using namespace Maptool;