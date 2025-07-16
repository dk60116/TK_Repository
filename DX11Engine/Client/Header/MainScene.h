#pragma once

#include "epch.h"

class CMainScene final : public CScene
{
public:
	CMainScene();
	~CMainScene();

public:
	HRESULT Initialize() override;
	void Update() override;

private:
	CCamera* m_pMainCamera;
	CLight* m_pDirLight;
	CCanvas* m_pCanvas;
	class CPlayer* m_pPlayer;
};

