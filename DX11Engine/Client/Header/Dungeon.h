#pragma once
#include "Map.h"

class CDungeon final : public CMap
{
	friend class CGameObject;

protected:
	explicit CDungeon();
	~CDungeon();

private:
	static CDungeon* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize() override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;
};

