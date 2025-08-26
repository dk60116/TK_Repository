#pragma once
#include "Component.h"

class CMapCollision final : public CComponent
{
	friend class CGameObject;

public:
	struct CollidersInfo
	{
		wstring name = L"";
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

public:
	void Set_Map(class CMap* _map);

private:
	CMap* m_pMap;
	map<wstring, CBoxCollider*> m_mColliderList;
	vector<CollidersInfo> _infoList;
};

