#pragma once

#include "framework.h"
#include "Define.h"
#include "struct.h"
#include "time.h"
#include "CTime.h"
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <functional>

#ifdef _DEBUG
#include "CConsole.h"
#endif

using namespace std;

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 
