#pragma once
#include "DungeonObject.h"

class CDungeonChest : public CDungeonObject
{
	friend class CGameObject;

protected:
	explicit CDungeonChest();
	~CDungeonChest();

public:
	static CDungeonChest* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize(void* _desc) override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnTriggerEnter(CCollider* _other) override;
	void OnTriggerExit(CCollider* _other) override;
	void OnDestroy() override;

public:
	void Set_Item(const wstring& _name, _uint _count = 1);
	void Open();

private:
	wstring m_strItemName;
	_uint m_iItemCount;
	_bool m_bDetacted, m_bIsOpen, m_bTakeItem;
	CTransform* m_pJoint;

	_float m_fOpenTimer;
};

