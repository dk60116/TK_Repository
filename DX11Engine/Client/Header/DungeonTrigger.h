#pragma once
#include "DungeonObject.h"

class CDungeonTrigger final : public CDungeonObject
{
	friend class CGameObject;

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
	void Set_Sibling(const _uint _index);
	void Set_LimitTime(const _float _time);
	void OffSwitch();
	void Set_DT();
	const _bool Get_SwitchOn() const;

private:
	void SwitchOnEvent();

private:
	_bool m_bSwitchOn, m_bSwitchComplete;
	vector<CDungeonObject*> m_vLinkObjects;
	CLight* m_pPointLight;
	CTexture* m_pOffTexture, * m_pOnTexture;
	_float m_fLimitTime, m_fPassedTime;
	_bool m_bDTSet;
};

