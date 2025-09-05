#pragma once
#include "Component.h"

class CBossMonster abstract : public CComponent
{
public:
	typedef struct BossMonsterDescription
	{

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

protected:
	wstring m_strBossName;
	vector<CSkinnedMeshRenderer*> m_vMeshRenderers;
	CTexture* m_pBaseMap;
	CAnimator* m_pAnimator;
	EngineAI::CNavMeshAgent* m_pNavAgent;

	map<wstring, class CMonsterPartCollision*> m_mPartColList;
};

