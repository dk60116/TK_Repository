#pragma once

#include "epch.h"

class CMonster abstract : public CComponent
{
	friend class CMonsterController;

public:
	struct MonsterOptions
	{
		vector3 colliderCenter = {};
		vector3 colliderSize = vector3::one();
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
	CMonster();
	~CMonster();

public:
	HRESULT Initialize() override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;

public:
	CAnimator* Get_Animator();
	void Change_State(const _uint _state);
	const MonsterStatus& Get_Status();

protected:
	CAnimationClip* Add_Animation(const wstring _name);

protected:
	wstring m_strSkinnedMeshBufferName;
	_float m_fSkinnedMeshScaleFactor;
	vector<CSkinnedMeshRenderer*> m_vMeshRenderers;
	CTexture* m_pBaseMap;
	CAnimator* m_pAnimator;
	class CMonsterController* m_pController;
	EngineAI::CNavMeshAgent* m_pNavAgent;

	MonsterOptions m_sOptions;
	MonsterStatus m_sStatus;

	CTransform* m_pHeadTF;

	CBoxCollider* m_pBodyCollider;
	CSphereCollider* m_pHeadCollider;
};

