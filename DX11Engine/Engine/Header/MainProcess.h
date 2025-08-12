#pragma once

#include "epch.h"

NS_BEGIN(Engine)

class ENGINE_DLL CMainProcess final
{
	SINGLETONCLASS(CMainProcess);

public:
	static HRESULT Initialize();
	static void Update_MainApp();
	static void Release_MainApp();
};

NS_END

