#pragma once
#include "BossController.h"

class CBossController_Dragon final : public CBossController
{
	friend class CGameObject;

public:
	enum DragonState { DIdle, Wakeup, Landing, DGetHit, RotateFly, ShootFireBall, Grounding, DTracking, Attack_Ground, DCombatWait, DDeath };

private:
	explicit CBossController_Dragon();
	~CBossController_Dragon();

private:
	static CBossController_Dragon* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize(void* _desc) override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;
};
