#pragma once
#include "Map.h"

class CDungeon final : public CMap
{
	friend class CGameObject;

protected:
	explicit CDungeon();
	~CDungeon();

private:
	static CDungeon* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize() override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void OnDestroy() override;

private:
	void SpawnMonsterPrototypes();
	void SpawnDungeonChapters();

private:
	vector<class CMonster*> m_vMonsterProtoList;
	vector<class CDungeonChapter*> m_vChapterList;

	_bool m_bAttachedChapterColliders;
};

