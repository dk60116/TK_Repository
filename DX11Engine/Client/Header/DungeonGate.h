#pragma once
#include "DungeonObject.h"

class CDungeonGate final : public CDungeonObject
{
protected:
	explicit CDungeonGate();
	~CDungeonGate();

public:
	static CDungeonGate* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize() override;

	void Awake() override;
	void Update() override;
	void OnDestroy() override;

public:
	void Open();
	void Close();

private:
	pair<_uint, _uint> m_iChapterIndex;

	_bool m_bIsOpen;
};

