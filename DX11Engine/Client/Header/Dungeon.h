#pragma once
#include "Map.h"

class CDungeon final : public CMap
{
protected:
	explicit CDungeon();
	~CDungeon();

public:
	static CDungeon* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize() override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;
};

