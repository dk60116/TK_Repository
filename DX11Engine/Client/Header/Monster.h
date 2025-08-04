#pragma once

#include "epch.h"

class CMonster abstract : public CComponent
{
	friend class CMonsterController;

public:
	struct MonsterStatus
	{
		_int maxHp = 3;
		_int crtHp = 0;
		_float detectionRange = 5.f;
		_float attackRange = 2.5f;
	};

protected:
	CMonster();
	~CMonster();

public:
	static CMonster* Create();
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
	CMonsterController* m_pController;

	MonsterStatus m_sStatus;
};

