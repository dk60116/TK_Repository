#pragma once

#include "epch.h"

class CGameScene final : public CScene
{
public:
	CGameScene();
	~CGameScene();

public:
	HRESULT Initialize() override;
	void Awake() override;
	void Update() override;

private:
	CCamera* m_pMainCamera;
	CLight* m_pDirLight;
	CCanvas* m_pCanvas;
	class CPlayer* m_pPlayer;
};

