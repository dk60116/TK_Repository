#pragma once
#include "DungeonObject.h"

class CDungeonChest : public CDungeonObject
{
protected:
	explicit CDungeonChest();
	~CDungeonChest();

public:
	static CDungeonChest* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize() override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnTriggerEnter(CCollider* _other) override;
	void OnTriggerExit(CCollider* _other) override;
	void OnDestroy() override;

public:
	void Open();

private:
	_bool m_bDetacted, m_bIsOpen;

	CTransform* m_pJoint;

	CBoxCollider* m_pBodyCollider;
};

