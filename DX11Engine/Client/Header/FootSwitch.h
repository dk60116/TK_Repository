#pragma once
#include "DungeonObject.h"

class CFootSwitch final : public CDungeonObject
{
	friend class CGameObject;

protected:
	explicit CFootSwitch();
	~CFootSwitch();

public:
	static CFootSwitch* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize(void* _desc) override;

	void Awake() override;
	void Update() override;
	void OnTriggerStay(CCollider* _other) override;
	void OnTriggerEnter(CCollider* _other) override;
	void OnTriggerExit(CCollider* _other) override;
	void OnDestroy() override;

public:
	void Set_Index(const _uint _index);
	void Set_Gate(vector<class CDungeonObject*>& _gates);

public:
	const _uint Get_Index() const;

private:
	void SwitchOnEvent();

private:
	vector<CDungeonObject*> m_vLinkObjects;

	CTransform* m_pBodyTransform;
	CSphereCollider* m_pStepCollider;

	_bool m_bObjectEnter;
	_bool m_bSwitchOn;
	_bool m_bSwitchComplete;

	_uint m_iSiblingSwitchIndex;
	_bool m_bPressed, m_bPrevPressed;

};

