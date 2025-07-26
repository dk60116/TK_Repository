#pragma once

#include "epch.h"

class CGameManager final
{
	SINGLETONCLASS(CGameManager);

public:
	void Set_NexScene(const wstring _scneName);
	const wstring& Get_NextScene() const;

private:
	wstring m_strNextScene;
};

