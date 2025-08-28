#pragma once

#include "epch.h"
#include "MonsterController.h"
#include "MonsterPartCollision.h"

class CMonster abstract : public CComponent
{
	friend class CMonsterController;

public:
	struct MonsterOptions
	{
		_float scaleFactor = 0.01f;
		vector3 colliderCenter = {};
		vector3 colliderSize = vector3::one();
		vector3 headColliderCenter = {};
		vector3 headColliderSize = vector3::one();
		vector3 bodyColliderCenter = {};
		vector3 bodyColliderSize = vector3::one();
	};

	struct MonsterStatus
	{
		_int maxHp = 3;
		_int crtHp = 0;
		_float moveSpeed = 3.f;
		_float rotateSpeed = 5.f;
		_float detectionRange = 10.f;
		_float attackWait = 3.f;
		_float attackRange = 3.f;
		_float walkSpeed = 2.f;
		_float runSpeed = 3.5f;
	};

protected:
	explicit CMonster();
	~CMonster();

public:
	HRESULT Initialize() override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnEnable() override;
	void OnCollisionEnter(class CCollider* _other) override;
	void OnDestroy() override;

public:
	const wstring& Get_MonsterName();
	const MonsterOptions& Get_Option() const;
	CAnimator* Get_Animator();
	CMonsterController* Get_Controller();
	void Change_State(const _uint _state);
	const MonsterStatus& Get_Status();
	void Get_Damage(class CWeapon* _weapon);
	void PlayTurn(const CMonsterController::TurnDir _dir);

protected:
	CAnimationClip* Add_Animation(const wstring _name);

protected:
	wstring m_strMonsterName;
	vector<CSkinnedMeshRenderer*> m_vMeshRenderers;
	CTexture* m_pBaseMap;
	CAnimator* m_pAnimator;
	CMonsterController* m_pController;
	EngineAI::CNavMeshAgent* m_pNavAgent;

	MonsterOptions m_sOptions;
	MonsterStatus m_sStatus;

	CTransform* m_pHeadTF, * m_pBodyTF;

	CBoxCollider* m_pBodyCollider;
	CRigidBody* m_pRigidBody;

	map<wstring, class CMonsterPartCollision*> m_mPartColList;
};

