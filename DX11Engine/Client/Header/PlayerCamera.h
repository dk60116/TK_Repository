#pragma once

#include "epch.h"
#include "Player.h"

class CPlayerCamera final : public CComponent
{
protected:
	CPlayerCamera();
	~CPlayerCamera();

public:
	static CPlayerCamera* Create();
	HRESULT Initialize() override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;

private:
	CPlayer* m_pPlayer;
};

