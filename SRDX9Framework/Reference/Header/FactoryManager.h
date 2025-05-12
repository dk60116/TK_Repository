#pragma once

#include "Engine_Define.h"

namespace Engine 
{
	class UObject;
}

BEGIN(Engine)

using CreateFunc = function<UObject*()>;

class ENGINE_DLL CFactoryManager
{
	SINGLETONCLASS(CFactoryManager);

public:
	unordered_map<string, CreateFunc>& getMap() { return m_mFactoryMap; }

private:
	unordered_map<string, CreateFunc> m_mFactoryMap;
};

END

