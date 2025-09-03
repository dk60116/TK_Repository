#pragma once
#include "DungeonObject.h"

class CFootSwitch final : public CDungeonObject
{
protected:
	explicit CFootSwitch();
	~CFootSwitch();

public:
	static CFootSwitch* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize() override;

	void Awake() override;
	void Update() override;
	void OnTriggerStay(CCollider* _other) override;
	void OnTriggerEnter(CCollider* _other) override;
	void OnTriggerExit(CCollider* _other) override;
	void OnDestroy() override;

public:
	void Set_Gate(vector<class CDungeonGate*>& _gates);

public:
	void OpenGate();

private:
	vector<CDungeonGate*> m_vGates;

	CTransform* m_pBodyTransform;
	CSphereCollider* m_pStepCollider;

	_bool m_bObjectEnter;
	_bool m_bPressed, m_bPrevPressed;;
};

