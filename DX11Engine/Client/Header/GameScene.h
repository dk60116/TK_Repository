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
	void SceneRelease() override;

	void ChangeBGM(const wstring& _clip);

private:
	class CPlayerCamera* m_pPlayerCamera;
	CLight* m_pDirLight;
	class CPlayer* m_pPlayer;

	class CPlayerHUD* m_pHUD;

	class CDungeon* m_pDungeon;
	CAudioSource* m_pBGMSource;
};

