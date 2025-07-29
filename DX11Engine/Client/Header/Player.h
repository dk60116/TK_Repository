#pragma once

#include "epch.h"

class CPlayer : public CComponent
{
public:
	enum PlayerAnimationStatus { Idle, Run, CombatIdle };

public:
	struct PlayerStatus
	{
		_uint maxHp = 3;
		_uint crtHp = 0;
		_float moveSpeed = 4.f;
		_float backWalkRatio = 0.7f;
		_float turnSpeed = 120.f;
		_uint attackPower = 1;
	};

protected:
	CPlayer();
	~CPlayer();

public:
	static CPlayer* Create();
	HRESULT Initialize() override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;

private:
	void PlayerControle();
	void PlayerControle_NoneLockOn();
	void PlayerControle_LockOn();
	void PlayerControle_AttackCombo();

	void PlayIdleAnimation();
	void PlayWalkAnimation();
	void PlayRunAnimation();
	void PlayBackWalkAnimation();
	
	void PlaySwordAnimation(_uint _index);

private:
	CSkinnedMeshRenderer* m_pSkinnedMeshRenderer;
	CAnimator* m_pAnimator;

	class CWeapon* m_pEquipWeapon;

	PlayerStatus m_sPlayerStatus;
	PlayerAnimationStatus m_eAnimationStatus;

	_float m_fCrtMoveSpeed;

	vector3 m_vMoveDirection, m_vPrevMoveDirectoin;
	_float m_fRotateDirection, m_fPrevRotateDirection;

	_bool m_bNotMoveTurning, m_bBackMove;
	_bool m_bLockOnMode, m_bIsCombatMode;
	_bool m_bPrevLockOnMode;
	_bool m_bIsAttack, m_bIsPrevAttack;
	_bool m_bSwordActionDuring;

	_float m_fSwordActionEndFrames[3];
};

