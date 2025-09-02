#pragma once
#include "DungeonObject.h"

class CDungeonChest : public CDungeonObject
{
protected:
	explicit CDungeonChest();
	~CDungeonChest();

public:
	static CDungeonChest* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize() override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;

public:
	void Open();

private:
	pair<_uint, _uint> m_iChapterIndex;

	_bool m_bLock;
	_bool m_bIsOpen;
};

