#pragma once
#include "DungeonObject.h"

class CDungeonCube final : public CDungeonObject
{
	friend class CGameObject;

protected:
	explicit CDungeonCube();
	~CDungeonCube();

public:
	static CDungeonCube* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize(void* _desc) override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnTriggerEnter(CCollider* _other) override;
	void OnDestroy() override;

private:
	CRigidBody* m_pRigidBody;
};

