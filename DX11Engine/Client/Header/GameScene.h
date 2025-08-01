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
	class CPlayerCamera* m_pPlayerCamera;
	CLight* m_pDirLight;
	CCanvas* m_pCanvas;
	class CPlayer* m_pPlayer;

	vector <class CMonster*> m_vMonsters;
};

