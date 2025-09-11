#pragma once
#include "Item.h"

class CItem_Bow final : public CItem
{
	friend class CInventory;

	explicit CItem_Bow();
	~CItem_Bow();

public:
	void OnGetEvent(const _uint _count) override;
	void OnDropEvent(const _uint _count) override;
	void OnUseEvent() override;
	void OnUnUseEvent() override;
};

