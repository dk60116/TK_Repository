#pragma once

#include "epch.h"

class CWeapon abstract : public CComponent
{
public:
	enum class WeaponType { None, Sword, Bow, Arrow, Bomb };

	struct WeaponOptions
	{
		vector3 localPos = {};
		quaternion localQuat = quaternion::identity();
		_float localScale = 1.f;
	};

	typedef struct WeaponStatus
	{
		WeaponType type = WeaponType::None;
		_int attack = 1;
		_float knockbackPower = 1.f;
	}WEAPONSTAT;

protected:
	explicit CWeapon();
	~CWeapon();

public:
	HRESULT Initialize() override;
	void Awake() override;

public:
	const WeaponType Get_WeaponType() const;
	const wstring& Get_WeaponName();
	const WEAPONSTAT& Get_Stat();
	void OnOffCollider(const _bool _value);

protected:
	wstring m_strWeaponName;
	WeaponOptions m_sOptions;
	WEAPONSTAT m_sStat;
	CMeshRenderer* m_pRenderer;
	CTransform* m_pTargetHand;
	CCollider *m_pCollider;
	CPlayer::HandType m_eHandType;
};

