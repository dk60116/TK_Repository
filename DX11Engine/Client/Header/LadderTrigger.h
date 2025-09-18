#pragma once
#include "DungeonObject.h"

class CLadderTrigger : public CDungeonObject
{
protected:
	explicit CLadderTrigger();
	~CLadderTrigger();

public:
	static CLadderTrigger* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize(void* _desc) override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnTriggerEnter(CCollider* _other) override;
	void OnDestroy() override;

public:
	void Set_Ladder(class CLadder* _ladder);

private:
	CLadder* m_pLadder;
	CBoxCollider* m_pCollider;
};

