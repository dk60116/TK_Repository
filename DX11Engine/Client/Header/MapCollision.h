#pragma once
#include "Component.h"

class CMapCollision final : public CComponent
{
	friend class CGameObject;

public:
	enum class MapCollisionType { Wall, Floor };

	struct CollidersInfo
	{
		BYTE type = 0;
		_int id = -1;
		_float4x4 matrix = {};
	};

private:
	explicit CMapCollision();
	~CMapCollision();

public:
	static CMapCollision* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize() override;

	void Awake() override;
	void Update() override;
	void OnDestroy() override;

public:
	HRESULT SaveColliders(const wstring _filePath);
	vector<CollidersInfo> ReadColliderInfo(const wstring _binFileName);
	void LoadColliders(const vector<CollidersInfo>& _info);

public:
	void Set_Map(class CMap* _map);

private:
	vector<CBoxCollider*> AbleColliderlist();

	CBoxCollider* SpawnTempCollider(const MapCollisionType _type);
	CBoxCollider* SpawnDataCollider(const CollidersInfo& _info);
	CBoxCollider* CopyTempCollider(CollidersInfo& _proto);

private:
	CMap* m_pMap;
	vector<CBoxCollider*> m_vColliderList;
	vector<CollidersInfo> m_vInfoList;

	_uint m_iIDCount;
};

