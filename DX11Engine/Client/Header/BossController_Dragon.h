#pragma once
#include "BossController.h"

class CBossController_Dragon final : public CBossController
{
	friend class CGameObject;

public:
	enum DragonState { Idle, Wakeup, Landing, RotateFly, ShootFireBall, SpreadFire };

private:
	explicit CBossController_Dragon();
	~CBossController_Dragon();

private:
	static CBossController_Dragon* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize() override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;
};
