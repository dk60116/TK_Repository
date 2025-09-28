#pragma once

#include "epch.h"
#include "PlayerController.h"

class CPlayer : public CComponent
{
public:
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
		_float moveSpeed = 6.f;
		_float jumpForce = 7.f;
		_float attackWalkRatio = 0.5f;
		_float backWalkRatio = 0.7f;
		_float turnSpeed = 180.f;
		_float aimingTurnSpeed = 120.f;
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
	HRESULT Initialize(void* _desc) override;
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
	CTransform* Get_Root() const;
	CTransform* Get_Hand(HandType _hand);
	void RecoverHp(const _uint _value);
	void GetDamage(const _uint _damage, const _bool _power = false);
	class CWeapon* Get_EqupWeapon() const;
	CWeapon* ChangeWeapon(const wstring _name);
	void ChangeArrow(const wstring _name);
	queue<class CArrow*>& Get_ArrowContainer(const wstring _name);

	void CloseSpinCollider();

public:
	void PlayIdleAnimation(const _bool _combat, const _float _blending = 0.2f);
	void PlayMoveAnimation(const vector3& _dir, const _float _rot, const _float _blending = 0.1f);
	void PlayJumpAnimation(const vector3& _dir, const _float _blending = 0.1f);
	
	void PlaySwordAnimation();
	void PlaySpinAttackAnimation();
	void PlayBowLoadAnimatoin();
	void PlayBowAimingAnimation();

	void PlayLadderAnimation(const _byte _dir, const _float _blending = 0.2f);
	void PlayLadderOutAnimation(const _float _blending = 0.2f);

	void PlaySoundEffect(const wstring& _clip);

	void AddHeart();

	void PopArrow();
	void ReturnArrow();
	void ShootArrow();

	void OnOffGravity(const _bool _on);

	const vector3& Get_GA();

	class CME_SwordSlash* Get_SwordSlashEffect();

private:
	CPlayerController* m_pController;
	class CInventory* m_pInventroy;
	CSkinnedMeshRenderer* m_pSkinnedMeshRenderer;
	CAnimator* m_pAnimator;
	CAudioSource* m_pAudioSource;

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

	_bool m_bSpinAttack;
	_float m_fSpinAttackGauge;

	CBoxCollider* m_pCollider;
	CRigidBody* m_pRigidBody;

	CTransform* m_pSpinParent;
	CBoxCollider* m_pSpinSwordTrigger;

	vector2Int m_vPrevMousePos;
	vector2 m_vMouseDragDelta;

	CME_SwordSlash* m_pSwordEffect;
};

