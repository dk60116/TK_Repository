#pragma once
#include "DungeonObject.h"

class CDungeonGate final : public CDungeonObject
{
protected:
	explicit CDungeonGate();
	~CDungeonGate();

public:
	static CDungeonGate* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize() override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnTriggerStay(CCollider* _other) override;
	void OnDestroy() override;

public:
	void SetLock();
	void Open();
	void Close();

private:
	CTransform* m_pBody;
	CBoxCollider* m_pBodyCollider;

	_bool m_bLock;
	_bool m_bIsOpen;

	CMeshRenderer* m_pLockRenderer;
	vector<CTransform*> m_vLockChainRenders;
};

