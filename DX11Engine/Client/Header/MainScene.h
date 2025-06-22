#pragma once

#include "epch.h"

class CMainScene final : public CScene
{
public:
	CMainScene();
	~CMainScene();

public:
	HRESULT Initialize() override;
};

