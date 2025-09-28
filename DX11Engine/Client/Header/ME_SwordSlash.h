#pragma once
#include "MeshEffect.h"

class CME_SwordSlash final : public CMeshEffect
{
protected:
	explicit CME_SwordSlash();
	~CME_SwordSlash();

public:
	static CME_SwordSlash* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize(void* _desc) override;
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnEnable() override;
	void OnDestroy() override;

public:
	void PlayCombo(const _uint _index);
};

