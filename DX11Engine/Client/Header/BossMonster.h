#pragma once
#include "Component.h"

class CBossMonster abstract : public CComponent
{
public:
	typedef struct BossMonsterDescription
	{
		_float scaleFactor = 0.01f;
	}BOSSDESC;

protected:
	explicit CBossMonster();
	~CBossMonster();

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
	CAnimator* Get_Animator() const;
	void Change_State(const _uint _state);
	void Get_Damage(class CWeapon* _weapon);

public:
	virtual void PlayIdle(const _float _blending = 0.2f) PURE;
	virtual void PlayThreat(const _float _blending = 0.2f) PURE;

protected:
	void Add_Animation(const wstring& _name);

protected:
	wstring m_strBossName;
	BOSSDESC m_sDesc;
	vector<CSkinnedMeshRenderer*> m_vMeshRenderers;
	CTexture* m_pBaseMap;
	CAnimator* m_pAnimator;

	map<wstring, class CMonsterPartCollision*> m_mPartColList;
};

