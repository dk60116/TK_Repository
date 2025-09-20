#pragma once
#include "DungeonObject.h"

class CDungeonTrigger final : public CDungeonObject
{
protected:
	explicit CDungeonTrigger();
	~CDungeonTrigger();

public:
	static CDungeonTrigger* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize(void* _desc) override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnTriggerEnter(CCollider* _other) override;
	void OnDestroy() override;

public:
	void Add_LinkObject(CDungeonObject* _obj);

private:
	void SwitchOnEvent();

private:
	vector<CDungeonObject*> m_vLinkObjects;
	CLight* m_pPointLight;
	CTexture* m_pOnTexture;
};

