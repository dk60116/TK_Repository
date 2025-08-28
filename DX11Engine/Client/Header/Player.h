#pragma once

#include "epch.h"
#include "PlayerController.h"

class CPlayer : public CComponent
{
public:
	enum PlayerAnimationStatus { Idle, Run, Jump, CombatIdle };
	enum class HandType { Left, Right };

public:
	struct StaticPlayerStatus
	{
		const static _uint HPMAX = 12;
	};

	struct PlayerStatus
	{
		_int maxHp = 6;
		_int crtHp = 0;
		_float moveSpeed = 4.f;
		_float jumpForce = 7.f;
		_float backWalkRatio = 0.7f;
		_float turnSpeed = 180.f;
		_float focusTurnRatio = 8.f;
		_int attackPower = 1;
		_float bowAimDragSpeed = 5.f;
	};

protected:
	explicit CPlayer();
	~CPlayer();

public:
	static CPlayer* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize() override;
	void Awake() override;
	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void OnDestroy() override;

	void OnCollisionEnter(class CCollider* _other) override;
	void OnCollisionStay(class CCollider* _other) override;
	void OnCollisionExit(class CCollider* _other) override;

public:
	CPlayerController* Get_Controller(); 
	CAnimator* Get_Animator() const;
	CRigidBody* Get_RigidBody() const;
	const PlayerStatus& Get_Status();
	CTransform* Get_Hand(HandType _hand);
	void Set_Focus(CTransform* _transform);
	void RecoverHp(const _uint _value);
	void GetDamage(const _uint _damage);
	class CWeapon* ChangeWeapon(const wstring _name);
	void ChangeArrow(const wstring _name);
	queue<class CArrow*>& Get_ArrowContainer(const wstring _name);

public:
	void PlayerControle();
	void PlayerControle_NoneLockOn();
	void PlayerControle_LockOn();
	void PlayerControle_AttackCombo();
	void PlayerControle_BowAction();

	void PlayIdleAnimation(const _float _blending = 0.1f);
	void PlayMoveAnimation(const vector3& _dir, const _float _rot, const _float _blending = 0.1f);
	void PlayJumpAnimation(const vector3& _dir, const _float _blending = 0.1f);
	
	void PlaySwordAnimation();
	void PlayBowLoadAnimatoin();

	const vector3& Get_GA();

private:
	CPlayerController* m_pController;
	CSkinnedMeshRenderer* m_pSkinnedMeshRenderer;
	CAnimator* m_pAnimator;

	CTransform* m_pRootTransform;
	CTransform* m_pRHandTransform, * m_pLHandTransform;

	map<wstring, class CWeapon*> m_mWeapons;
	CGameObject* m_pArrowProto;
	map<wstring, queue<class CArrow*>> m_mArrowPool;
	wstring m_strCrtArrow;
	vector<CArrow*> m_vUsedArrows;
	CArrow* m_pEquipArrow;
	CWeapon* m_pEquipWeapon;

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
	_bool m_bBowLoadDuring, m_bPrevBowLoadDuring, m_bShootReady;

	_float m_fSwordActionEndFrames[3];

	_float m_fAttackComboNT, m_fBowLoadingNT;
	_uint m_iAttackComboDest;

	_bool m_bIsJump, m_bIsPrevJump;

	CTransform* m_pFocusTransform;

	EngineAI::CNavMeshAgent* m_pNavAgent;
	CBoxCollider* m_pCollider;
	CRigidBody* m_pRigidBody;

	vector2Int m_vPrevMousePos;
	vector2 m_vMouseDragDelta;
};

