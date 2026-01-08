#pragma once
#include "DungeonObject.h"

class CDungeonGate final : public CDungeonObject
{
	friend class CGameObject;

protected:
	explicit CDungeonGate();
	~CDungeonGate();

protected:
	static CDungeonGate* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize(void* _desc) override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnEnable() override;
	void OnTriggerStay(CCollider* _other) override;
	void OnDestroy() override;

public:
	void SetLock();
	void Open();
	void Close();

private:
	CTransform* m_pBody;

	_bool m_bLock;
	_bool m_bUnLock;
	_bool m_bIsOpenDoor, m_bIsOpenLock;
	_bool m_bComplete;

	CMeshRenderer* m_pLockRenderer;
	vector<CTransform*> m_vLockChainRenders;

	_float m_fPassedTime;
};

