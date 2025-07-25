#pragma once

#include "epch.h"

class LoadingScene final : public CScene
{
public:
	LoadingScene();
	~LoadingScene();

public:
	HRESULT Initialize() override;
	void Update() override;

private:
	CCamera* m_pMainCamera;
	CCanvas* m_pCanvas;
};

