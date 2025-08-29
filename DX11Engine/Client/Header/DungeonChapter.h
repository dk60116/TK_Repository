#pragma once
#include "Component.h"
#include "Monster.h"

class CDungeonChapter final : public CComponent
{
	friend class CGameObject;

public:
	typedef struct DungeonChapterBoundingBoxDescription
	{
		pair<vector3, vector3> posScale = pair(vector3::zero(), vector3::one() * 10.f);
		_float yRot = 0.f;

	} BOUNDINGBOXDESC;

	typedef struct DungeonChapterMonsterDescription
	{
		CMonster* prototype;
		_uint count;
		vector<vector3> position;

	} MONSTERSPAWNER;

private:
	explicit CDungeonChapter();
	~CDungeonChapter();

private:
	static CDungeonChapter* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize() override;
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;

	void OnTriggerEnter(CCollider* _other) override;
	void OnTriggerExit(CCollider* _other) override;

public:
	void SetBoundingBox(const BOUNDINGBOXDESC& _desc);

public:
	void OnPlayerEnter();
	void OnPlayerExit();

private:
	void Bind_BoundingBox();
	void AttachColliders();

private:
	class CDungon* m_pDungeon;
	CBoxCollider* m_pBoundingBox;
	vector<CGameObject*> m_vGateList;
	BOUNDINGBOXDESC m_sBoundingBoxInfo;
	vector<MONSTERSPAWNER> m_vMonsterList;
	vector<CBoxCollider*> m_vMapColList;
};

