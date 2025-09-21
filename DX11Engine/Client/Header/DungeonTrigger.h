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
	const _uint Get_Index() const;
	void Set_Sibling(const _uint _index);
	void Set_LimitTime(const _float _time);

private:
	void SwitchOnEvent();
	void OffSwitch();

private:
	_uint m_iSiblingSwitchIndex;
	_bool m_bSwitchOn, m_bSwitchComplete;
	vector<CDungeonObject*> m_vLinkObjects;
	CLight* m_pPointLight;
	CTexture* m_pOffTexture, * m_pOnTexture;
	_float m_fLimitTime, m_fPassedTime;
};

