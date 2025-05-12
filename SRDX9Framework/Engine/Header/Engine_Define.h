#ifndef Engine_Define_h__
#define Engine_Define_h__

#include <d3d9.h>
#include <d3dx9.h>

#include <fmod.h>
#include <fmod_common.h>
#include <fmod_errors.h>
#include <fmod.hpp>

#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <functional>
#include <string>
#include <unordered_map>
#include <ctime>
#include <typeindex>
#include <memory>
#include <atomic>
#include <dwmapi.h>
#include <windowsx.h>
#pragma comment(lib, "dwmapi.lib")

#include "ProjectResource.h"
#include "Engine_Enum.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_Typedef.h"
#include "Engine_Function.h"

#define DIRECTINPUT_VERSION	0x0800
#include <dinput.h>

#define TRAVERSAL_ITER(p, n) auto n = p.begin(); n != p.end(); ++n

#define MAINTOPBARHEIGHT 40
#define CHILDTOPBARHEIGHT 28

#define HWND_BASETOPBAR 100
#define HWND_CHILDTOPBAR 101
#define HWND_BASEBOTTOMBAR 102
#define HWND_HIERACHYSIDE 103
#define HWND_INPUTBOX 104
#define HWND_INSPECTORCOMPONENTTOP 105
#define HWND_INSPECTORCOMPONENTBODY 106
#define HWND_CHECKBOX 107

#ifndef TVM_GETHOTITEM               
#   define TVM_GETHOTITEM (TV_FIRST + 58)
#endif

#ifndef TreeView_GetHotItem
#   define TreeView_GetHotItem(hwnd) \
        (HTREEITEM)SendMessage((hwnd), TVM_GETHOTITEM, 0, 0)
#endif

#define BEGIN_SERIALIZEFIELD \
public: \
std::vector<FieldInfo> GetInspectorFields() override { \
std::vector<FieldInfo> _fields;

#define SERIALIZEFIELD(var) \
_fields.push_back({std::wstring(L#var).substr(3), DetectFieldType(var), &var});

#define END_SERIALIZEFIELD \
return _fields; } \

#pragma warning(disable : 4251)

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif

using namespace std;
using namespace Engine;

#endif // Engine_Define_h__
