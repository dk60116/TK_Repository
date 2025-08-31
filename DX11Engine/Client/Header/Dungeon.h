#pragma once
#include "Map.h"
#include "Monster.h"

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
	template <typename T>
	CMonster* CrateMonsterPrototype();

private:
	map<wstring, class CMonster*> m_mMonsterProtoList;
	vector<class CDungeonChapter*> m_vChapterList;

	_bool m_bAttachedChapterColliders;
};

template <typename T>
inline CMonster* CDungeon::CrateMonsterPrototype()
{
	CGameObject* monsterObj = m_pGameObject->Get_Scene()->Add_GameObject(L"Monster Clone");
	CMonster* monster = monsterObj->AddComponent<T>();
	monsterObj->Set_ObjectName(L"Prototype_" + monster->Get_MonsterName());
	m_mMonsterProtoList.emplace(monster->Get_MonsterName(), monster);

	monsterObj->SetActive(false);

	return nullptr;
}
