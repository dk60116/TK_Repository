#pragma once
#include "BossMonster.h"
#include "BossController_Dragon.h"

class CDragon final : public CBossMonster
{
public:
	struct DragonStatus : BossStatus
	{
		_uint fireballPower = 1;
		_uint handPower = 2;

		_float flyingTurnSpeed = 5.f;
		_float riseSpeed = 1.5f;
		_float glideSpeed = 15.f;
		_float fireballInterval = 4.f;
	};

	struct LandingDesc
	{
		_bool threadRise;
		vector3 targetPos;
	};

protected:
	explicit CDragon();
	~CDragon();

public:
	static CDragon* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize() override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnEnable() override;
	void OnCollisionEnter(class CCollider* _other) override;
	void OnDestroy() override;

public:
	CBossController_Dragon* Get_Controller();
	const DragonStatus& Get_Status();

public:
	void Get_Damage(class CWeapon* _weapon) override;

	void PlayIdle(const _float _blending = 0.2f) override;
	void PlayThreat(const _float _blending = 0.2f) override;
	void PlayGroundToFly(const _float _blending = 0.2f);
	void PlayFly(const _float _blending = 0.2f);
	void PlayGetHit(const _float _blending = 0.2f);
	void PlayGlide(const _float _blending = 0.2f);
	void PlayShootFireball(const _float _blending = 0.2f);
	void PlaySpreadFire(const _float _blending = 0.2f);

	void ShootFireBall();
	const _uint Get_ShootFireBallCount() const;

	const _bool GetFlying() const;
	void SetFlying(const _bool _fly);

	const _uint Get_ShootFireCount() const;

private:
	DragonStatus m_sStatus;
	CBossController_Dragon* m_pController;
	_bool m_bFlying;
	class CME_FireBall* m_pFireBallProto;
	_uint m_iShootFireCount;
};

