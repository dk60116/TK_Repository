#pragma once
#include "Map.h"
#include "Monster.h"
#include "DungeonObject.h"

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
	void SpawnDungonObjectPrototypes();
	void SpawnDungeonChapters();

	void CreateDungeonGates();
	void CreateDungeonFootSwitches();
	void CreateDungeonChest();
	void CreateDungeonLadder();

private:
	template <typename T>
	CMonster* CrateMonsterPrototype();
	template <typename T>
	CDungeonObject* CreateDungonObjectPrototype();

private:
	map<wstring, CMonster*> m_mMonsterProtoList;
	map<wstring, CDungeonObject*> m_mDungonObjProtoList;
	vector<class CDungeonChapter*> m_vChapterList;

	_bool m_bAttachedChapterColliders;

	vector<class CDungeonGate*> m_vGateList;
	vector<class CFootSwitch*> m_vFootSwitchList;
	vector<class CDungeonChest*> m_vChestList;
	vector<class CLadder*> m_vLadderList;
};

template <typename T>
inline CMonster* CDungeon::CrateMonsterPrototype()
{
	CGameObject* monsterObj = m_pGameObject->Get_Scene()->Add_GameObject(L"Monster Clone");
	CMonster* monster = monsterObj->AddComponent<T>();
	monsterObj->Set_ObjectName(L"Prototype_" + monster->Get_MonsterName());
	m_mMonsterProtoList.emplace(monster->Get_MonsterName(), monster);

	monsterObj->SetActive(false);

	return monster;
}

template<typename T>
inline CDungeonObject* CDungeon::CreateDungonObjectPrototype()
{
	CGameObject* objectObj = m_pGameObject->Get_Scene()->Add_GameObject(L"Dungon object Clone");
	CDungeonObject* obj = objectObj->AddComponent<T>();
	objectObj->Set_ObjectName(L"Prototype_" + obj->Get_ObjName());
	m_mDungonObjProtoList.emplace(obj->Get_ObjName(), obj);

	objectObj->SetActive(false);

	return obj;
}
