#pragma once
#include "DungeonObject.h"

class CLadder final : public CDungeonObject
{
protected:
	explicit CLadder();
	~CLadder();

public:
	static CLadder* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize() override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnTriggerStay(CCollider* _other) override;
	void OnDestroy() override;

private:
	CBoxCollider* m_pBodyCollider;
};

