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
	void OnDestroy() override;

public:
	void Set_Gate(vector<class CDungeonGate*>& _gates);

private:
	vector<CDungeonGate*> m_pGates;
};

