#pragma once

#define WIN32_LEAN_AND_MEAN            
// Windows 헤더 파일
#include <windows.h>

// DirectX 헤더 파일
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#pragma comment(lib, "d3dcompiler.lib")
using namespace DirectX;
using Microsoft::WRL::ComPtr;

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// 커스텀 헤더파일
#include "MainProcess.h"
using namespace std;
