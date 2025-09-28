#pragma once
#include "Monster.h"

class CBossMonster abstract : public CMonster
{
public:
	typedef struct BossMonsterDescription
	{
		_float scaleFactor = 0.01f;
	}BOSSDESC;

	struct BossStatus
	{
		_uint maxHp = 12;
		_uint crtHp = 12;
		_float runSpeed = 5.f;
		_float attackRange = 10.f;
		_float rotateSpeed = 2.f;
	};

protected:
	explicit CBossMonster();
	~CBossMonster();

public:
	HRESULT Initialize(void* _desc) override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnEnable() override;
	void OnCollisionEnter(class CCollider* _other) override;
	void OnDestroy() override;

public:
	const wstring& Get_MonsterName();
	CAnimator* Get_Animator() const;
	void Change_State(const _uint _state);
	void Get_Damage(class CWeapon* _weapon);
	const _bool Is_Dead() const;

public:
	virtual void PlayIdle(const _float _blending = 0.2f) PURE;
	virtual void PlayThreat(const _float _blending = 0.2f) PURE;

protected:
	BOSSDESC m_sDesc;

	map<wstring, class CMonsterPartCollision*> m_mPartColList;
};

