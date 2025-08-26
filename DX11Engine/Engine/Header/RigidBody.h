#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CRigidBody final : public CComponent
{
	friend class CGameObject;

public:
	struct RigidBodyOptions
	{
		_float mass = 1.f;
		_float drag = 1.f;;
	};

	struct FreezeOption
	{
		_bool x, y, z = false;
	};

protected:
	explicit CRigidBody();
	~CRigidBody();

private:
	static CRigidBody* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize() override;
	void Awake() override;
	void Update() override;
	void LateUpdate() override;
	void Render_Editor() override;
	void Render() override;
	void OnDestroy() override;

public:
	const _bool UseGravity() const;
	void SetUseGravity(const _bool _value);
	const _bool IsKinematic() const;
	void SetKinematic(const _bool _value);

	void AddForce(const vector3 _value);

	void ResetVelocity();
	void ResetGravity();

	void SetVelocity(const vector3 _value);
	void AddVelocity(const vector3 _value);
	void AddVelocitX(const _float _value);
	void AddVelocitY(const _float _value);
	void AddVelocitZ(const _float _value);

private:
	class CCollider* m_pCollider;

	RigidBodyOptions m_sOptions;
	FreezeOption m_sFreezePosition;
	FreezeOption m_sFreezeRotation;
	_bool m_bUseGravity, m_bIsKinematic;
	vector3 m_vVelocity, m_vGravityAcceleration;
};

NS_END

