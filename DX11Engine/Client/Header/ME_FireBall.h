#pragma once
#include "MeshEffect.h"

class CME_FireBall final : public CMeshEffect
{
	friend class CGameObject;

protected:
	explicit CME_FireBall();
	~CME_FireBall();

public:
	static CME_FireBall* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize(void* _desc) override;
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnTriggerEnter(CCollider* _other) override;
	void OnDestroy() override;

public:
	void Shoot(const vector3& _startPos, const vector3& _targetPos);

private:
	void Shooting();

private:
	CLight* m_pLight;

	_float m_fRotYSpeed;

	_bool m_bShooting;
	vector3 m_vTargetPos, m_vDirection;

	CSphereCollider* m_pCollider;
	CParticleSystem* m_pParticle;
};

