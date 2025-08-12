#pragma once

#include "epch.h"

class CWeapon abstract : public CComponent
{
public:
	enum class WeaponType { Swords, Bow, Bomb };

	struct WeaponOptions
	{
		vector3 localPos = {};
		quaternion localQuat = quaternion::identity();
		_float localScale = 1.f;
	};

protected:
	explicit CWeapon();
	~CWeapon();

public:
	HRESULT Initialize() override;
	void Awake() override;

protected:
	wstring m_strWeaponName;
	WeaponOptions m_sOptions;
	CMeshRenderer* m_pRenderer;
	CTransform* m_pTargetHand;
	CBoxCollider* m_pCollider;
};

