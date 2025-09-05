#pragma once
#include "BossMonster.h"
#include "BossController_Dragon.h"

class CDragon final : public CBossMonster
{
public:
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

public:
	void PlayIdle(const _float _blending) override;
	void PlayThreat(const _float _blending) override;
	void PlayGroundToFly(const _float _blending = 0.2f);

	const _bool GetFlying() const;
	void SetFlying(const _bool _fly);

private:
	CBossController_Dragon* m_pController;
	_bool m_bFlying;
};

