#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CResources
{
	SingletonClass(CResources);

public:
	template<typename T>
	void CreateResource(wstring _name, wstring _path);

private:
};

END

