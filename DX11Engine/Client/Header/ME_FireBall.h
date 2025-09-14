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
	HRESULT Initialize() override;
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;

private:
	_float m_fRotYSpeed;
	_float m_fTime;
};

