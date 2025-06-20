#pragma once

#include "Object.h"

NS_BEGIN(Engine)

class ENGINE_DLL CMainProcess final : public UObject
{
	SINGLETONCLASS(CMainProcess);

public:
	HRESULT Initialize() override;
	void Update_MainApp();
};

NS_END

